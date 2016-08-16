#ifndef MEETINGEEBLOCK_H
#define MEETINGEEBLOCK_H

#include <functional>

#include <QtGui/qtextcursor.h>
#include <QtCore/qvector.h>

#include "qrwidgets_global.h"

NS_QRWIDGETS_BEGIN

struct QrMailBlock {
    //	'name@domain.com' to name<name@domain.com>;
    static QString formatDisplayByName(const QString& name);

    //	@param offset
    //  the positive to expand and the negative to narrow
    bool isCursorPosIn(int cursorPos, int offset = 0) const;
    bool operator==(const QrMailBlock &rhs) const;

    QString name;       //  actual input, name@domain.com
    QString display;    //  display to textedit, name<name@domain.com>
    int beginPos;   //  begin curpor position in textedit
    int endPos;     //  end curpor position in textedit
    int nextPos;    //  begin curpor position in textedit of next mail
};

class QRWIDGETSSHARED_EXPORT QrMailboxBlockContainer {
private:
    typedef QVector<QrMailBlock> MeetingeeBlockVector;

public:
    //  format : name@domain.com; name@domain.com;
    QStringList getNames();

    //
    int size() const;
    const QrMailBlock& lastBlock() const;

    //
    bool contains(const QrMailBlock& value);
    bool containName(const QString& name);

    void push(const QrMailBlock& value);
    //  remove the block by block's display
    void removeByDisplay(const QString& display);
    /**
     * @brief remove the block which it contain the specifed postion
     * @param postionContained  the specifed postion which block should be contain
     * @param offset    the positive to expand and the negative to narrow
     */
    void removeByInsidedPosition(int postionContained, int offset = 0);
    //  remove the block by block's nextPos
    void removeByNextedPosition(int postionNexted);

    //  get the block by block's nextPos(begin position of the behinded block)
    bool previousBlock(int postionNexted, QrMailBlock *value);
    /*!
     * \brief get the block which it contain the specifed postion
     * \param postionContained  the specifed postion which block should be contain
     * \param value
     * \param offset    the positive to expand and the negative to narrow
     * \return
     */
    bool currentBlock(int postionContained, QrMailBlock *value, int offset = 0);
    //
    void updatePosWhenInputOneKey(int keyPosition);
    //
    void updatePosWhenDeleteOneKey(int keyPosition);

private:
    bool find(std::function<bool(QrMailBlock)> func, QrMailBlock *value);
    void updatePositionBeforeRemove(const QrMailBlock& removed);
    /*!
     * \brief	update block's position when key press
     * \param keyPosition
     * \param inputOrDelete	true for input, false for delete
     */
    void updatePositionWhenKeyPress(int keyPosition, bool inputOrDelete = true);

private:
    MeetingeeBlockVector meetingeeBlocks;
};

NS_QRWIDGETS_END

#endif // MEETINGEEBLOCK_H
