#include "emailinputter/qrmailboxblock.h"

#include <algorithm>

USING_NS_QRWIDGETS;

QString QrMailBlock::formatDisplayByName(const QString &name) {
    return name.left(name.indexOf('@'))
            + "<"
            + name
            + ">;";
}

bool QrMailBlock::isCursorPosIn(int cursorPos, int offset /* = 0*/) const {
    return cursorPos >= (beginPos - offset)
            && cursorPos <= (endPos + offset);
}

bool QrMailBlock::operator==(const QrMailBlock &rhs) const {
    return name == rhs.name
            && display == rhs.display
            && beginPos == rhs.beginPos
            && endPos == rhs.endPos
            && nextPos == rhs.nextPos;
}

////////////////////////////////////////////
QStringList QrMailboxBlockContainer::getNames()
{
    QStringList meetingees;
    Q_FOREACH(const QrMailBlock& elem, meetingeeBlocks) {
        meetingees.push_back(elem.name);
    }
    return meetingees;
}

bool QrMailboxBlockContainer::contains(const QrMailBlock &value) {
    return meetingeeBlocks.contains(value);
}

bool QrMailboxBlockContainer::containName(const QString &name)
{
    return meetingeeBlocks.end() !=
            std::find_if(meetingeeBlocks.begin(), meetingeeBlocks.end(),
                         [name](const QrMailBlock& elem){
        return elem.name == name;
    });
}

void QrMailboxBlockContainer::push(const QrMailBlock &value) {
    meetingeeBlocks.push_back(value);
}

void QrMailboxBlockContainer::removeByDisplay(const QString &display)
{
    QrMailBlock blockFind;
    if (find([display](const QrMailBlock& elem){
                      return (elem.display == display);
                  }, &blockFind)) {
        updatePositionBeforeRemove(blockFind);
        meetingeeBlocks.removeOne(blockFind);
    }
}

void QrMailboxBlockContainer::removeByInsidedPosition(int postionContained,
                                                    int offset /*= 0*/)
{
    QrMailBlock blockFind;
    if (currentBlock(postionContained, &blockFind, offset)) {
        updatePositionBeforeRemove(blockFind);
        meetingeeBlocks.removeOne(blockFind);
    }
}

void QrMailboxBlockContainer::removeByNextedPosition(int postionNexted)
{
    QrMailBlock blockFind;
    if (previousBlock(postionNexted, &blockFind)) {
        updatePositionBeforeRemove(blockFind);
        meetingeeBlocks.removeOne(blockFind);
    }
}

bool QrMailboxBlockContainer::previousBlock(int postionNexted,
                                          QrMailBlock *value)
{
    return find([postionNexted](const QrMailBlock& elem){
        return (elem.nextPos == postionNexted);
    }, value);
}

bool QrMailboxBlockContainer::currentBlock(int postionContained,
                                         QrMailBlock *value,
                                         int offset /*= 0*/)
{
    return find([postionContained, offset](const QrMailBlock& elem){
        return elem.isCursorPosIn(postionContained, offset);
    }, value);
}

bool QrMailboxBlockContainer::find(std::function<bool (QrMailBlock)> func, QrMailBlock *value) {
    auto blockFind = std::find_if(std::end(meetingeeBlocks), std::begin(meetingeeBlocks), func);
    if ( blockFind != std::begin(meetingeeBlocks) ) {
        *value = *blockFind;
        return true;
    }

    return false;
}

void QrMailboxBlockContainer::updatePositionBeforeRemove(const QrMailBlock& removed)
{
    auto updateIdx = meetingeeBlocks.indexOf(removed);
    if (-1 == updateIdx) {
        return;
    }

    QrMailBlock tempParam = removed;
    QrMailBlock *preBlock = &tempParam;
    for(++updateIdx; updateIdx < meetingeeBlocks.size(); ++updateIdx) {
        QrMailBlock& curBlock = meetingeeBlocks[updateIdx];

        int width = curBlock.endPos - curBlock.beginPos;
        curBlock.beginPos = preBlock->beginPos;
        curBlock.endPos = curBlock.beginPos + width;
        curBlock.nextPos = curBlock.endPos + 1;

        preBlock = &curBlock;
    }
}
