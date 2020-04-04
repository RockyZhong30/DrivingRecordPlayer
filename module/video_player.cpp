#include "video_player.h"
#include "module/signdetector.h"
#include <iostream>
#include <string>
#include <vector>
#include "mainwindow.h"

Video_Player::Video_Player(QSlider *slider,  QLabel *video_label, QLabel *label_pos, QLabel *label_total, QLabel *img_0, QLabel *img_1, QLabel *img_2, QLabel *img_3, QLabel *img_4, QLabel *img_5)
{
    //构造函数
    this->slider = slider;
    this->video_label = video_label;
    this->label_pos=label_pos;
    this->label_total=label_total;

    this->img_0 = img_0;
    this->img_1 = img_1;
    this->img_2 = img_2;
    this->img_3 = img_3;
    this->img_4 = img_4;
    this->img_5 = img_5;

    //初始化打开视频文件标志
    m_openvideo_flag = false;
    //初始化暂停检测标志
    m_pause_flag = false;
    //初始化开始检测标志
    m_start_flag = false;
    //初始化停止检测标志
    m_stop_flag = false;

    m_runFlag = false;

}


void Video_Player::show_img(Mat image,QLabel *label){

//    在label上显示opencv图片
    cv::cvtColor(image,image,CV_BGR2RGB);
    QImage img = QImage((const unsigned char*)(image.data),image.cols,image.rows,
                        image.cols*image.channels(),
                        QImage::Format_RGB888).scaled(label->width(), label->height());

//    label->clear();
    label->setPixmap(QPixmap::fromImage(img));
              //ui->processPushButton->setEnabled(true);

}

bool Video_Player::play(QString path)
{
    m_stop_flag = false;
    capture.open(path.toStdString());
    if(!capture.isOpened())
    {
        return false;
    }
    rate = capture.get(CAP_PROP_FPS);
    frameNum = capture.get(CAP_PROP_FRAME_COUNT);
//    int second = frameNum/rate;
    slider->setRange(0,capture.get(CAP_PROP_FRAME_COUNT));
    this->label_total->setText(QString::number(capture.get(CAP_PROP_FRAME_COUNT)));

    return true;
}
void Video_Player::Stop()
{
    //停止播放
    m_stop_flag=true;
}

int Video_Player::Faster()
{
    if(speed>5)
    {
       speed-=5;
       return speed;
    }
    else
    {
        return 5;
    }


}
int Video_Player::Slower(){
    if(speed<1000){
       speed+=20;

       return speed;
    }
    else{
        return 1000;
    }

}

void Video_Player::Start()
{
    //打开播放锁
    m_stop_flag=false;
}

void Video_Player::Pase()
{
    //暂停
    m_pause_flag = true;

}
void Video_Player::PaseOrStart()
{
    //暂停
    if(m_pause_flag == true)
    {
        m_pause_flag = false;
    }
    else
    {
        m_pause_flag = true;
    }

}
double Video_Player::get_current_pos()
{
    return (capture.get(0));
}

QString Video_Player::formatTime(int second)
{

}

void Video_Player::showSaveImage(Mat &roi)
{
//    //拼接图片文件名
//    string fileName = "signimages/"+to_string(m_num)+".jpg";
//    //保存图片
//    imwrite(fileName,roi);
//    //图片号加1
//    m_num++;
//    //显示roi
//    selectLabelShow(fileName);

    QString fileName = GlobalVar::IMG_SAVE_DIR_PATH+"/"+QString::number(m_pictureNum)+".jpg";
    imwrite(fileName.toStdString(), roi);

//    qDebug() << "-----------------------fileName:" << fileName << m_pictureNum;
//    MainWindow::getObj()->updateImg(fileName, m_pictureNum);
//    QPixmap pixmap(fileName);
////    QMetaObject::invokeMethod(img_0, "setPixmap", Qt::QueuedConnection, Q_ARG(QPixmap, pixmap));
//    img_0->setPixmap(pixmap);

    QPixmap pixmap(fileName);
    int value = m_pictureNum % 6;
    switch (value)
    {
    case 1:
        img_0->setPixmap(pixmap);
        break;
    case 2:
        img_1->setPixmap(pixmap);
        break;
    case 3:
        img_2->setPixmap(pixmap);
        break;
    case 4:
        img_3->setPixmap(pixmap);
        break;
    case 5:
        img_4->setPixmap(pixmap);
        break;
    case 0:
        img_5->setPixmap(pixmap);
        break;
    default:
        break;
    }

    m_pictureNum++;
}

void Video_Player::Set_postion(long po){
    //根据frame进行设置位置
    this->pos = po;
}
void Video_Player::run()
{
    //player的线程函数

    /*****创建SignDetector类对象*****/
    SignDetector signdetector;
    m_sign_count = 0;           //初始化检测到的交通标志的总个数
    int last_count = 0;         //上一次检测到交通标志的个数
    int cur_count = 0;          //当前检测到交通标志的个
    bool isZero = false;        //是否没有检测到交通标志
    bool first = true;          //是否第一次检测到交通标志
    m_pictureNum = 1;

    while(m_runFlag)
    {
        cv::waitKey(25);
        //点击进度条响应 变化起始位置
        if(pos!=-1)
        {
          capture.set(CAP_PROP_POS_FRAMES, pos);
          pos=-1;
        }

        if(m_stop_flag)
        {
            m_runFlag = false;
            break;
        }

        //暂停检测
        if(m_pause_flag)
        {
            continue;
        }

        bool ret = capture.read(frame);
        if(!ret)
        {
            continue;
        }

        //重设大小,有利于后续的位置(选择遮罩的位置)
        cv::resize(frame, frame, cv::Size(640,368), 0, 0);

        /*********下面是：基于RGB的交通标志检测*********/

        //色彩分割
        Mat matRgb = signdetector.colorSegmentation(frame);
        // 中值滤波
        matRgb = signdetector.deNoise(matRgb);
        //形态学处理
        matRgb = signdetector.morphologyProcess(matRgb);
        //找轮廓
        std::vector<std::vector<Point>> contours;  //轮廓
        std::vector<Rect> boundRect = signdetector.myfindContours(matRgb,contours);
        //保存找到的交通标志的下标
        std::vector<int> index;
        //记住这个变量要重新置0，否则会加上之前的个数，出错
        cur_count =0;
        //tmp给roi截取
        Mat tmp = frame.clone();
        //首先进行一定的限制，筛选出轮廓
        for (int i = 0; i < contours.size(); i++)
        {
            Rect rect = boundRect[i];
            //若轮廓矩形内部还包含着矩形，则将被包含的小矩形取消
            bool inside = false;
            for (int j = 0; j < contours.size(); j++)
            {
                Rect t = boundRect[j];
                if (rect == t)
                    continue;
                else if (signdetector.isInside(rect, t))
                {
                    inside = true;
                    break;
                }
            }
            if (inside)
                continue;
            //高宽比限制
            float ratio = (float)rect.width / (float)rect.height;
            //轮廓面积限制
            //float Area = (float)rect.width * (float)rect.height;
            float dConArea = (float)contourArea(contours[i]);
            //float dConLen = (float)arcLength(contours[i], 1);
            if (dConArea < 700)
                continue;
            if(ratio > 3)
                continue;
            //画轮廓识别框
            Scalar color = (0, 0, 255);//蓝色线画轮廓
            rectangle(frame, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);
            //画一个，当前个数加1
            cur_count++;
            //保存相应轮廓的下标
            index.push_back(i);
        }

        //cur_count不为0，则有检测到
        if(cur_count)
        {
            //(1)第一次检测到，直接将cur_count赋值给sign_count
            if(first)
            {
                m_sign_count = cur_count;
                //将第一次检测标志置为false
                first =false;
                //将没有检测到标志置false
                isZero = false;
                //将检测到图像显示到label,并保存
                for(int i=0; i< cur_count;i++)
                {
                    Mat roi = tmp(Rect(boundRect[index[i]].tl().x,boundRect[index[i]].tl().y,boundRect[index[i]].width,boundRect[index[i]].height));
                    showSaveImage(roi);
                }
            }
            else
            {
                //非第一次检测
                if(isZero == true)//(2)如果没有检测到后，再一次检测到,则直接加上cur_count
                {
                    m_sign_count += cur_count;
                    //将没有检测到标志置false
                    isZero = false;

                    //将检测到图像显示到label,并保存
                    for(int i=0; i< cur_count;i++)
                    {
                        Mat roi = tmp(Rect(boundRect[index[i]].tl().x,boundRect[index[i]].tl().y,boundRect[index[i]].width,boundRect[index[i]].height));
                        showSaveImage(roi);
                    }
                }
                else if(cur_count > last_count)  //(3)由检测1个，持续当前检测变成2时，则增加了1个
                {
                    m_sign_count += (cur_count-last_count);
                    //将检测到图像显示到label,并保存
                    for(int i=0; i<(cur_count-last_count);i++)
                    {
                        Mat roi = tmp(Rect(boundRect[index[i]].tl().x,boundRect[index[i]].tl().y,boundRect[index[i]].width,boundRect[index[i]].height));
                        showSaveImage(roi);
                    }
                }
            }
            //将当前的个数赋值给上一次
            last_count = cur_count;
        }
        else
        {
            //将没有检测到标志置true
            isZero = true;
        }
        index.erase(index.begin(),index.end());

        long i = capture.get(CAP_PROP_POS_FRAMES);


//        //暂停响应
//       if (!Is_Pase)
//       {
//           bool ret = capture.read(frame);
//            if(Stop_Play){
//                break;
//            }
//            if(!ret){
//                continue;
//            }
//            show_img(frame,video_label);
//            QTest::qSleep(speed);
//       }

        //将图像显示到label中
        show_img(frame, video_label);
        QMetaObject::invokeMethod(slider, "setValue", Qt::QueuedConnection, Q_ARG(int, i));
        QMetaObject::invokeMethod(label_pos, "setText", Qt::QueuedConnection,Q_ARG(QString,QString::number(i)));

    }
}
