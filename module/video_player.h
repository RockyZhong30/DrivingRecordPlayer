#ifndef VIDEO_PLAYER_H
#define VIDEO_PLAYER_H

#include <QThreadPool>
#include <QProgressBar>
#include <QThread>
#include <QDebug>
#include <string.h>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QSlider>
#include <opencv2/opencv.hpp>
//#include <QTest>
#include <QLabel>
#if 0
#include "settingList/pagemovetableview.h"
#endif
#include "global/globalvar.h"
using namespace cv;

class Video_Player : public QRunnable
{
public:
    Video_Player(QSlider *slider,  QLabel *video_label, QLabel *label_pos, QLabel *label_total, QLabel *img_0, QLabel *img_1, QLabel *img_2, QLabel *img_3, QLabel *img_4, QLabel *img_5);
    bool play(QString path);
    void run();
    void Stop();
    void Start();
    int Faster();
    int Slower();
    void Pase();
    void PaseOrStart();
    void Set_postion(long po);
    double get_current_pos();
    QString formatTime(int second);

public:
    bool m_runFlag;                 //线程运行标志

    bool m_openvideo_flag;          //打开视频文件是否成功标志
    bool m_pause_flag;               //暂停检测标志
    bool m_start_flag;              //开始检测标志
    bool m_stop_flag;                //停止检测标志
    int m_sign_count;               //检测到的交通标志的总个数
    int m_pictureNum;               //检测图片号

    int rate;
    int frameNum;

protected:
    void showSaveImage(cv::Mat &roi);        //显示保存检测到的交通标志图片

private:
    QSlider* slider;
    QString Video_Path;
    QLabel *video_label;
    QLabel *label_pos;
    QLabel *label_total;

    QLabel *img_0;
    QLabel *img_1;
    QLabel *img_2;
    QLabel *img_3;
    QLabel *img_4;
    QLabel *img_5;

    VideoCapture capture;
    void show_img(Mat src,QLabel *label);
    Mat frame;

    int speed =30;
    int pos=-1;
};

#endif // VIDEO_PLAYER_H
