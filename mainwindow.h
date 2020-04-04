#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMouseEvent>
#if 0
#include "settingList/pagemovetableview.h"
#endif
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
#if 0
    KeyCoordinateDelegate *m_keyCoodinateDelegate;
    KeyCoordinateModel *m_pageModel;
#endif

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

#if 0
    void pageViewClick(QModelIndex index);
#endif

    void on_horizontalSlider_sliderMoved(int position);

public slots:
    void updateImg(QString imgName, int imgNum);
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
