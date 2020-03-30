#include "mainsettingdelegate.h"
#include <QPainter>
//#include "common.h"
MainSettingDelegate::MainSettingDelegate(QObject *parent, int type) :
    QItemDelegate(parent)
{
    mType = type;
}
void MainSettingDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    painter->setPen(Qt::black);
    QRect rectItem = option.rect;
    if (option.state & QStyle::State_Selected)
    {
        painter->setBrush(QColor(2,124,152));
        painter->setPen(Qt::white);
        painter->drawRect(rectItem);
    }
    QRect iconRect(rectItem.left()+7,rectItem.top()+12,25,rectItem.height()/2);
    rectItem.setLeft(iconRect.left()+25);
    painter->drawPixmap(iconRect,QPixmap(index.data(Qt::DecorationRole).toString()));
    QString name;
        name = index.data(Qt::DisplayRole).toString();
    QImage  tag;
    if (option.state & QStyle::State_Selected)
    {
        tag = QImage(":/res/setting/settings-left-menu-arrow-activation.png");
    }
    else
    {
        tag = QImage(":/res/setting/settings-left-menu-arrow.png");
    }
    int offset = (rectItem.height()-tag.size().height())/2;
    QFont f("DejaVu Sans",20);
    painter->setFont(f);
    QRect nameRect = rectItem;
    nameRect.setLeft(nameRect.left()+10);
    painter->drawText(nameRect,Qt::AlignVCenter,name);
    painter->drawImage(rectItem.right()-20,rectItem.top()+offset,tag);
    painter->setPen(QColor(23,23,23));
    painter->drawLine(option.rect.left(),option.rect.bottom()-1,option.rect.right(),option.rect.bottom()-1);
    painter->setPen(QColor(83,83,83));
    painter->drawLine(option.rect.left(),option.rect.bottom(),option.rect.right(),option.rect.bottom());
    painter->drawLine(option.rect.bottomLeft(),option.rect.bottomRight());
    painter->restore();
}

QSize MainSettingDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    int item_height = 52;
//    switch(SCREEN_WIDTH)
//    {
//        default:
//        case 800:
//            break;
//        case 1280:
//        case 1366:
//            item_height = 72;
//            break;
//    }
    return QSize(0,item_height);
}
