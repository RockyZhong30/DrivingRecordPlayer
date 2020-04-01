#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMouseEvent>

#include "settingList/pagemovetableview.h"
#include "module/video_player.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static MainWindow *getObj();

public:
    KeyCoordinateDelegate *m_keyCoodinateDelegate;
    KeyCoordinateModel *m_pageModel;

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
    void btnPlayStartClick();
    void btnStopClick();

    void pageViewClick(QModelIndex index);

    void on_horizontalSlider_sliderMoved(int position);

public slots:
    void updateTableViewImg(KeyCoordinateModel::Group_ST st);
private:
    void initUi();
    void initConnect();
    void initPara();
private:
    QFont iconFont;             //图形字体

    Video_Player *player;

private:
    Ui::MainWindow *ui;
    static MainWindow *mMainWindow;
};

#endif // MAINWINDOW_H
