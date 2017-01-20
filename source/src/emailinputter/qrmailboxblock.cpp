#include "emailinputter/qrmailboxblock.h"

#include <algorithm>

#include <QtCore/qdebug.h>

#include "qrutf8.h"

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
QStringList QrMailboxBlockContainer::getNames() const
{
    QStringList names;
    Q_FOREACH(const QrMailBlock& elem, meetingeeBlocks) {
        names.push_back(elem.name);
    }
    return names;
}

QStringList QrMailboxBlockContainer::getDisplayes(bool chopSemicon) const
{
    QStringList displayes;
    Q_FOREACH(const QrMailBlock& elem, meetingeeBlocks) {
        auto display = elem.display;
        if (chopSemicon) {
            display.chop(1);
        }
        displayes.push_back(display);
    }
    return displayes;
}

void QrMailboxBlockContainer::clear()
{
    meetingeeBlocks.clear();
}

int QrMailboxBlockContainer::size() const
{
    return meetingeeBlocks.size();
}

const QrMailBlock &QrMailboxBlockContainer::lastBlock() const
{
    return meetingeeBlocks.last();
}

bool QrMailboxBlockContainer::contains(const QrMailBlock &value) {
    return meetingeeBlocks.contains(value);
}

bool QrMailboxBlockContainer::containDisplayName(const QString &displayName)
{
    return meetingeeBlocks.end() !=
            std::find_if(meetingeeBlocks.begin(), meetingeeBlocks.end(),
                         [displayName](const QrMailBlock& elem){
        return elem.display == displayName;
    });
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

void QrMailboxBlockContainer::updatePosWhenInputOneKey(int keyPosition)
{
    updatePositionWhenKeyPress(keyPosition, true);
}

void QrMailboxBlockContainer::updatePosWhenDeleteOneKey(int keyPosition)
{
    updatePositionWhenKeyPress(keyPosition, false);
}

bool QrMailboxBlockContainer::find(std::function<bool (QrMailBlock)> func, QrMailBlock *value) {
    auto blockFind = std::find_if(meetingeeBlocks.begin(), meetingeeBlocks.end(), func);
    if ( blockFind != meetingeeBlocks.end() ) {
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

    int beginPosOfPreBlock = removed.beginPos;
    for(++updateIdx; updateIdx < meetingeeBlocks.size(); ++updateIdx) {
        QrMailBlock& curBlock = meetingeeBlocks[updateIdx];

        int width = curBlock.endPos - curBlock.beginPos;
        curBlock.beginPos = beginPosOfPreBlock;
        curBlock.endPos = curBlock.beginPos + width;
        curBlock.nextPos = curBlock.endPos + 1;

        beginPosOfPreBlock = curBlock.nextPos;
    }
}

void QrMailboxBlockContainer::updatePositionWhenKeyPress(int keyPosition, bool inputOrDelete) {
    if (meetingeeBlocks.isEmpty()) {
        return;
    }

    int findIdx = 0;
    if (keyPosition > meetingeeBlocks.first().beginPos) {
        for (; findIdx < meetingeeBlocks.count() - 1; ++findIdx) {
            auto left = meetingeeBlocks.at(findIdx);
            auto right = meetingeeBlocks.at(findIdx+1);
            if (left.endPos < keyPosition
                    && right.beginPos >= keyPosition) { //  (left.endPos, right.beginPos]
                ++findIdx;
                break;
            }
        }

        if (findIdx == meetingeeBlocks.count()) {
            qDebug() << "update error" << keyPosition;
            return;
        }
    }

    for (; findIdx < meetingeeBlocks.count(); ++findIdx) {
        auto& curBlock = meetingeeBlocks[findIdx];
        if (inputOrDelete) {    //  input
            ++curBlock.beginPos;
            ++curBlock.endPos;
            ++curBlock.nextPos;
        } else {    //  delete
            --curBlock.beginPos;
            --curBlock.endPos;
            --curBlock.nextPos;
        }
    }
}
