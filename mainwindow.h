﻿#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMouseEvent>

#include "settingList/pagemovetableview.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    QPoint m_movePoint; //移动的距离
    bool m_mousePress; //按下鼠标左键

private slots:
    void menuCloseClick();
    void btnSettingClick();
    void btnOpenClick();
private:
    void initUi();
    void initConnect();
private:
    QFont iconFont;             //图形字体

    KeyCoordinateDelegate *m_keyCoodinateDelegate;
    KeyCoordinateModel *m_pageModel;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
