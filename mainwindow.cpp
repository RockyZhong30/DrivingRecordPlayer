#include <QFontDatabase>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#define SCREEN_HEIGHT 550
#define SCREEN_PLAY_WIDTH 700
#define SCREEN_IMG_WEIDTH 300

MainWindow *MainWindow::mMainWindow = NULL;

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->initUi();
    this->initConnect();
    this->initPara();

}

MainWindow::~MainWindow()
{
    if(NULL != player)
    {
        player->m_runFlag = false;
        if(QThreadPool::globalInstance()->waitForDone())
        {
            player = NULL;
        }
    }
    delete ui;
}

MainWindow *MainWindow::getObj()
{
    if(mMainWindow == NULL)
        mMainWindow = new MainWindow();
    return mMainWindow;
}

void MainWindow::initUi()
{
    this->setWindowFlags(Qt::FramelessWindowHint /*| Qt::WindowStaysOnTopHint*/);
    this->setFixedSize(SCREEN_PLAY_WIDTH, SCREEN_HEIGHT);
    ui->frame_play->setFixedWidth(SCREEN_PLAY_WIDTH);
    ui->frame_img->setFixedWidth(SCREEN_IMG_WEIDTH);
    ui->stackedWidget->setCurrentWidget(ui->page_bg);
    ui->frame_img->hide();

    int fontId = QFontDatabase::addApplicationFont(":/res/image/fontawesome-webfont.ttf");
    QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);
    iconFont = QFont(fontName, 17);

    ui->label_title->setFont(iconFont);
    ui->label_title->setText(QChar(0xf01d));

    ui->btnMenu_Close->setFont(iconFont);
    ui->btnMenu_Close->setText(QChar(0xf00d));

    ui->btn_open->setFont(iconFont);
    ui->btn_open->setText(QChar(0xf07c));
    ui->btn_play_pause->setFont(iconFont);
    ui->btn_play_pause->setText(QChar(0xf04b));
//    ui->btn_pause->setFont(iconFont);
//    ui->btn_pause->setText(QChar(0xf04c));
    ui->btn_stop->setFont(iconFont);
    ui->btn_stop->setText(QChar(0xf04d));
    ui->btn_setting->setFont(iconFont);
    ui->btn_setting->setText(QChar(0xf0c9));

//    ui->btn_left->setFont(iconFont);
//    ui->btn_left->setText(QChar(0xf0d9));
//    ui->btn_right->setFont(iconFont);
//    ui->btn_right->setText(QChar(0xf0da));

    iconFont.setPointSize(111);
    ui->label_bgicon->setFont(iconFont);
    ui->label_bgicon->setText(QChar(0xf01d));
    ui->label_bginfo->setText("Designed by xu.rz");

//    ui->tableView->setLabelPage(ui->label_page);
    m_keyCoodinateDelegate = new KeyCoordinateDelegate(this);
    m_pageModel = new KeyCoordinateModel(this);
    ui->tableView->setModel(m_pageModel);
    ui->tableView->setItemDelegate(m_keyCoodinateDelegate);
    ui->tableView->takeParaSlot();
}

void MainWindow::initConnect()
{
    connect(ui->btnMenu_Close, SIGNAL(clicked()), this, SLOT(menuCloseClick()));
    connect(ui->btn_setting, SIGNAL(clicked()), this, SLOT(btnSettingClick()));
    connect(ui->btn_open, SIGNAL(clicked()), this, SLOT(btnOpenClick()));
    connect(ui->btn_play_pause, SIGNAL(clicked()), this, SLOT(btnPlayStartClick()));
    connect(ui->btn_stop, SIGNAL(clicked()), this, SLOT(btnStopClick()));

//    connect(ui->btn_left, SIGNAL(clicked()), ui->tableView, SLOT(pageUp()));
//    connect(ui->btn_right, SIGNAL(clicked()), ui->tableView, SLOT(pageDown()));

    connect(ui->tableView, SIGNAL(pressed(QModelIndex)), this, SLOT(pageViewClick(QModelIndex)));
}

void MainWindow::initPara()
{
    player = NULL;
}

void MainWindow::menuCloseClick()
{
    this->close();
}

void MainWindow::btnSettingClick()
{
    if(ui->frame_img->isHidden())
    {
        ui->tableView->takeParaSlot();
        ui->frame_img->show();
        this->setFixedSize(SCREEN_IMG_WEIDTH+SCREEN_PLAY_WIDTH, SCREEN_HEIGHT);
    }
    else
    {
        ui->frame_img->hide();
        this->setFixedSize(SCREEN_PLAY_WIDTH, SCREEN_HEIGHT);
    }
}

void MainWindow::btnOpenClick()
{
    QString fileName = QFileDialog::getOpenFileName(
               this, QStringLiteral("选择要播放的文件"),
                "",//初始目录
                QStringLiteral("视频文件 (*.flv *.rmvb *.avi *.MP4 *.mkv);;")
                +QStringLiteral("音频文件 (*.mp3 *.wma *.wav);;")
                +QStringLiteral("所有文件 (*.*)"));
    if (!fileName.isEmpty())
    {
        QFileInfo fileInfo(fileName);
        ui->label_file->setText("{"+fileInfo.suffix()+"}  "+fileInfo.fileName());

        if(NULL == player)
        {
            //当没有视频播放时
             player = new Video_Player(ui->horizontalSlider, ui->label_play, ui->label_currenttime, ui->label_totaltime);
             if(player->play(fileName))
             {
                 ui->tableView->m_allPages = player->frameNum/9;

//                 m_pageModel = new KeyCoordinateModel(this, 3, 3, player->frameNum/9);
                 ui->tableView->setModel(m_pageModel);
                 ui->tableView->pageToFirst();
                 ui->btn_play_pause->setText(QChar(0xf04c));
                 player->m_start_flag = true;
                 //开启线程
                 player->m_runFlag = true;
                 QThreadPool::globalInstance()->start(player);
                 ui->stackedWidget->setCurrentWidget(ui->page_play);
             }
        }
        else
        {
            //当有视频播放时
            player->Stop();
            if(player->play(fileName))
            {
                ui->btn_play_pause->setText(QChar(0xf04c));
            }
        }
    }
}

void MainWindow::btnPlayStartClick()
{
    if(NULL != player)
    {
        //暂停
        if(player->m_start_flag)
        {
            if(player->m_pause_flag)
            {
                player->m_pause_flag = false;
                ui->btn_play_pause->setText(QChar(0xf04c));
            }
            else
            {
                player->m_pause_flag = true;
                ui->btn_play_pause->setText(QChar(0xf04b));
            }
        }
    }
}

void MainWindow::btnStopClick()
{
    if(NULL != player)
    {
        player->m_stop_flag = true;
        player->m_start_flag = false;
        ui->stackedWidget->setCurrentWidget(ui->page_bg);
        ui->btn_play_pause->setText(QChar(0xf04b));
    }
}

void MainWindow::pageViewClick(QModelIndex index)
{
    if(index.isValid()==false)
    {
        return;
    }

//    qDebug() << "name:" << index.data(Qt::DisplayRole).toString() << "num:" << index.data(Qt::DecorationRole).toInt();
}

void MainWindow::updateTableViewImg(KeyCoordinateModel::Group_ST st)
{
    m_pageModel->addgroupStData(st);
//    qDebug() << "-----------------name:" << st.name << "num" << st.num;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    //只能是鼠标左键移动和改变大小
    if(event->button() == Qt::LeftButton)
    {
        m_mousePress = true;
    }
    //窗口移动距离
    m_movePoint = event->globalPos() - pos();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *)
{
    m_mousePress = false;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    //移动窗口
    if(m_mousePress)
    {
        QPoint movePpos = event->globalPos();
        move(movePpos - m_movePoint);
    }
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    //拖动进度条响应 PaseOrStart在这里非常重要,实现了拖动时的动态变化
    if(NULL != player)
    {
        player->Pase();
        ui->btn_play_pause->setText(QChar(0xf04b));
        player->Set_postion(ui->horizontalSlider->value());
    }
}
