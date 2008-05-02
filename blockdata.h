#ifndef BLOCKDATA_H
#define BLOCKDATA_H

#include <QTextBlock>
#include <QTextBlockUserData>

struct BlockData : public QTextBlockUserData
{
    static BlockData *data(const QTextBlock &block)
    { return static_cast<BlockData *>(block.userData()); }
    void setToBlock(QTextBlock &block)
    { block.setUserData(this); }
    QList<int> code;
};

#endif
