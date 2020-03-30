#ifndef MAINSETTINGDELEGATE_H
#define MAINSETTINGDELEGATE_H
#include <QItemDelegate>
class MainSettingDelegate : public QItemDelegate
{
public:
    MainSettingDelegate(QObject *parent = 0,int type = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
private:
    int mType;
};
#endif // MAINSETTINGDELEGATE_H
