﻿#ifndef PAGEMOVETABLEVIEW_H
#define PAGEMOVETABLEVIEW_H
#include <QTableView>
#include <QLabel>
#include <QItemDelegate>
#include <QPainter>



class KeyCoordinateDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit KeyCoordinateDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
private:

};


class KeyCoordinateModel : public QAbstractListModel
{
    Q_OBJECT
public:
    KeyCoordinateModel(QObject *parent, int rows=3, int columns=3, int pages=1);

    int dataCount() const;
    int columnCount(const QModelIndex &) const;
    int rowCount(const QModelIndex &) const;
    QVariant data(const QModelIndex &index, int role) const;

public:
    struct Group_ST
    {
        int num;
        QString name;
        QImage image;
    };

public:
    void addgroupStData(Group_ST st);

public:
    QVector<Group_ST> m_groupStVec;
    int m_pages; //显示的总页数
    int m_rows;
    int m_columns;

protected:


private:


public slots:
    void readData(QString path);

private:
//    int m_group;
//    QStringList m_stringList;
};


class PageMoveTableView : public QTableView
{
    Q_OBJECT
public:
    explicit PageMoveTableView(QWidget *parent = 0);

    void setLabelPage(QLabel *page);
    void pageToFirst();

public:
    int m_rowCount;
    int m_columnCount;
    int m_currPage;//当前页
    int m_allPages;//总页数


private:


signals:

public slots:
    void initUi();
    void takePara();
    void takeParaSlot();
    void pageUp();
    void pageDown();
    void showLabelPage();

private:
    QLabel *m_labelPage;
    QScrollBar *m_qScrollBar;

};
#endif // PAGEMOVETABLEVIEW_H
