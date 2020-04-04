#include "pagemovetableview.h"
#include <QMouseEvent>
#include <QDebug>
#include <QScrollBar>
#include <QHeaderView>
#include <QApplication>
#include <QTimer>
#include <QLayout>
#include <QMargins>
#include <QFile>
#include "qdom.h"
#include "qdebug.h"


KeyCoordinateDelegate::KeyCoordinateDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

void KeyCoordinateDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
#undef MARGIN
#define MARGIN    2
    QRect rectItem = option.rect;
    QRect border;
    border = QRect(rectItem.left()+MARGIN,rectItem.top()+MARGIN,rectItem.width()-MARGIN*2,rectItem.height()-MARGIN*2);

    QString name = index.data(Qt::DisplayRole).toString();
//    qDebug() << "-----------name:" << name;
    QFile file(name);
    QLinearGradient linearGrad(QPointF(rectItem.left(), rectItem.top()), QPointF(rectItem.left(), rectItem.bottom()));

    if(file.exists())
    {
        painter->drawPixmap(border, QPixmap(name));
    }
    else
    {
//        painter->drawPixmap(border, QPixmap(":/res/image/model.jpg"));
        linearGrad.setColorAt(0, QColor(88,88,88));
        linearGrad.setColorAt(1, QColor(88,88,88));
        QBrush brush(linearGrad);
        painter->setBrush(brush);
        painter->setPen(QColor(45,1,1));
        painter->drawRoundedRect(border,8.0,8.0);
    }
    if(option.state & QStyle::State_Selected)
    {
//        linearGrad.setColorAt(0, QColor(255,153,102));
//        linearGrad.setColorAt(1, QColor(255,153,102));
//        QBrush brush(linearGrad);
//        painter->setBrush(brush);
//        painter->setPen(QColor(45,1,1));
//        painter->drawRoundedRect(border,8.0,8.0);
    }
    else
    {
//        linearGrad.setColorAt(0, QColor(88,88,88));
//        linearGrad.setColorAt(1, QColor(88,88,88));
//        QBrush brush(linearGrad);
//        painter->setBrush(brush);
//        painter->setPen(QColor(45,1,1));
//        painter->drawRoundedRect(border,8.0,8.0);
    }
    //        if(index.data(KeyCoordinateModel::KeyCoordinate_page).toInt()==-1)
    //            return;
    QRect nameRect = border;
    #ifdef  INCH_10_LIKE
    QFont f("DejaVu Sans",28);
    #else
    QFont f("DejaVu Sans",20);
    #endif

    if(option.state & QStyle::State_Selected)
    {
     painter->setPen(Qt::black);
    }
    else
    {
    painter->setPen(Qt::white);
    }
    //        painter->setFont(f);
    //        QString name = index.data(Qt::DisplayRole).toString();
    //        nameRect.setLeft(nameRect.left()+2);
    //        nameRect.setRight(nameRect.right()-2);
    //        int nameRectWidth=nameRect.width();
    //        QFontMetrics fontMetrics(f);
    //        int strWidth;
    //        int count=name.count();
    //        int allWidth=fontMetrics.width(name);
    //        if(allWidth<=nameRectWidth)
    //        {
    //            nameRect.setBottom(nameRect.top()+(nameRect.bottom()-nameRect.top()));
    //            painter->drawText(nameRect,Qt::AlignCenter,name);
    //        }
    //        else
    //        {
    //            QString s1;
    //            QString s2;
    //            int i=5;
    //            for(;i<=count;i++)
    //            {
    //                strWidth=fontMetrics.width(name.left(i));
    //                if(strWidth>nameRectWidth)
    //                {
    //                    s1=name.left(i-1);
    //                    s2=name.remove(0,i-1);
    //                    break;
    //                }
    //            }
    //            int h = (nameRect.bottom()-nameRect.top())*1*0.5;
    //            nameRect.setBottom(nameRect.top()+h);
    //            painter->drawText(nameRect,Qt::AlignCenter,s1);
    //            nameRect.setTop(nameRect.bottom()+1);
    //            nameRect.setBottom(nameRect.top()+h);
    //            if(fontMetrics.width(s2)<nameRectWidth)
    //                painter->drawText(nameRect,Qt::AlignCenter,s2);
    //            else
    //                painter->drawText(nameRect,Qt::AlignLeft|Qt::AlignVCenter,s2);
    //        }
}


KeyCoordinateModel::KeyCoordinateModel(QObject *parent, int rows, int columns, int pages) :
    QAbstractListModel(parent)
  ,m_rows(rows)
  ,m_columns(columns)
  ,m_pages(pages)
{

    m_groupStVec.clear();
}

void KeyCoordinateModel::readData(QString path)
{
    QFile xml(path);

    if(!xml.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

    QDomDocument doc;

    if(!doc.setContent(&xml, false))
    {
        return;
    }

    m_groupStVec.clear();
    int num = -1;

    QDomNode root = doc.documentElement().firstChildElement("layout");
    QDomNodeList children = root.childNodes();

    for(int i = 0; i != children.count(); ++i)
    {
        QDomElement nodeInfo = children.at(i).toElement();
//        qDebug() << "--------label:" << nodeInfo.attribute("label");

        QDomNodeList secondLevel = nodeInfo.childNodes();
        num ++;

        for(int j = 0; j != secondLevel.count(); ++j)
        {
            QDomElement secNodeInfo = secondLevel.at(j).toElement();
//            qDebug() << "--------decond label:" << secNodeInfo.attribute("label");
//            qDebug() << "--------decond num:" << secNodeInfo.attribute("num");
//            m_stringList.append(secNodeInfo.attribute("label"));
            num++;
            Group_ST st;
            st.name = secNodeInfo.attribute("label");
            st.num = secNodeInfo.attribute("num").toInt();
            m_groupStVec.append(st);
        }
    }
}

QVariant KeyCoordinateModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int column = index.column();

    qDebug() << "--------------m_groupStVec.count" << m_groupStVec.count();

    if(!index.isValid() || m_groupStVec.isEmpty())
    {
        return QVariant();
    }

    int value = row * (m_columns) + column;

    if(role == Qt::DisplayRole)//文字
    {
        qDebug() << "---------row:" << row << "column:" << column << "role:" << role;
        if(value < m_groupStVec.count())
        {
            return m_groupStVec.at(value).name;
        }
    }

    if(role == Qt::DecorationRole)
    {
        if(value < m_groupStVec.count())
        {
            return m_groupStVec.at(value).num;
        }
        else
        {
            int val = -1;
            return val;
        }
    }
    return QVariant();
}

int KeyCoordinateModel::dataCount() const
{
    return m_rows * m_pages *m_columns;
}

int KeyCoordinateModel::rowCount(const QModelIndex &) const
{
    return m_rows * m_pages;
}

int KeyCoordinateModel::columnCount(const QModelIndex &) const
{
    return m_columns;
}

void KeyCoordinateModel::addgroupStData(Group_ST st)
{
//    beginResetModel();
//    qDebug() << "------name:" << st.name << "num:" << st.num;
//    m_groupStVec.append(st);
//    qDebug() << "-------count:" << m_groupStVec.count();
//    endResetModel();
}



PageMoveTableView::PageMoveTableView(QWidget *parent) :
    QTableView(parent)
{
    this->initUi();
    m_qScrollBar = this->verticalScrollBar();
    m_qScrollBar->setMaximum(rect().bottom());
    m_qScrollBar->setMinimum(rect().top());
    m_rowCount = 3;
    m_columnCount = 3;
    m_allPages = 1;
    m_labelPage = NULL;
}

void PageMoveTableView::initUi()
{
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->setFocusPolicy(Qt::StrongFocus);
    this->setAcceptDrops(true);

    this->setFrameShape(QFrame::NoFrame);
    this->setLineWidth(0);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setProperty("showDropIndicator", QVariant(false));
    this->setDragDropOverwriteMode(false);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->setShowGrid(false);
    this->setGridStyle(Qt::NoPen);
    this->horizontalHeader()->setVisible(false);
    this->horizontalHeader()->setHighlightSections(false);
    this->verticalHeader()->setVisible(false);
    this->verticalHeader()->setHighlightSections(false);
}

void PageMoveTableView::setLabelPage(QLabel *page)
{
    m_labelPage = page;
}

void PageMoveTableView::pageToFirst()
{
    m_qScrollBar->triggerAction(QAbstractSlider::SliderToMinimum);
    m_currPage = 0;
    showLabelPage();
}

void PageMoveTableView::takePara()
{
    int width=this->geometry().width()-(this->geometry().width()%m_columnCount);
    int height=this->geometry().height()-(this->geometry().height()%m_rowCount);
//    qDebug() << "++++++++++++++++++++++++++++++++++++++++width:" << width << "height:" << height;
    this->horizontalHeader()->setDefaultSectionSize(width/m_columnCount);
    this->verticalHeader()->setDefaultSectionSize(height/m_rowCount);
    this->setGeometry(this->geometry().x(),this->geometry().y(),width,height);
    int max = height*(m_allPages-1);
    m_qScrollBar->setRange(0,max);
    m_qScrollBar->setPageStep(156);
}

void PageMoveTableView::takeParaSlot()
{
    takePara();
    QTimer::singleShot(66, this, SLOT(takePara()));
}

void PageMoveTableView::pageUp()
{
    m_qScrollBar->triggerAction(QAbstractSlider::SliderPageStepSub);
    m_currPage = (m_qScrollBar->value() + m_qScrollBar->pageStep()/2)/m_qScrollBar->pageStep();
    showLabelPage();
}

void PageMoveTableView::pageDown()
{
    m_qScrollBar->triggerAction(QAbstractSlider::SliderPageStepAdd);
    m_currPage = (m_qScrollBar->value() + m_qScrollBar->pageStep()/2)/m_qScrollBar->pageStep();
    showLabelPage();
}

void PageMoveTableView::showLabelPage()
{
    if(NULL != m_labelPage)
    {
        if(m_allPages==0)
            m_labelPage->setText("0/0");
        else
        {
            m_labelPage->setText(QString::number(m_currPage+1)+"/"+QString::number(m_allPages));
        }
    }
}
