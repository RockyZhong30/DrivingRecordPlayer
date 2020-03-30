#include <QFontDatabase>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#define SCREEN_HEIGHT 550
#define SCREEN_PLAY_WIDTH 700
#define SCREEN_IMG_WEIDTH 300

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->initUi();
    this->initConnect();

}

MainWindow::~MainWindow()
{
    delete ui;
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
    ui->btn_play->setFont(iconFont);
    ui->btn_play->setText(QChar(0xf04b));
    ui->btn_pause->setFont(iconFont);
    ui->btn_pause->setText(QChar(0xf04c));
    ui->btn_stop->setFont(iconFont);
    ui->btn_stop->setText(QChar(0xf04d));
    ui->btn_setting->setFont(iconFont);
    ui->btn_setting->setText(QChar(0xf0c9));

    ui->btn_left->setFont(iconFont);
    ui->btn_left->setText(QChar(0xf0d9));
    ui->btn_right->setFont(iconFont);
    ui->btn_right->setText(QChar(0xf0da));

    iconFont.setPointSize(111);
    ui->label_bgicon->setFont(iconFont);
    ui->label_bgicon->setText(QChar(0xf01d));
    ui->label_bginfo->setText("Designed by xu.rz");

    ui->tableView->setLabelPage(ui->label_page);
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

    connect(ui->btn_left, SIGNAL(clicked()), ui->tableView, SLOT(pageUp()));
    connect(ui->btn_right, SIGNAL(clicked()), ui->tableView, SLOT(pageDown()));
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
    QString s = QFileDialog::getOpenFileName(
               this, QStringLiteral("选择要播放的文件"),
                "",//初始目录
                QStringLiteral("视频文件 (*.flv *.rmvb *.avi *.MP4 *.mkv);;")
                +QStringLiteral("音频文件 (*.mp3 *.wma *.wav);;")
                +QStringLiteral("所有文件 (*.*)"));
    if (!s.isEmpty())
    {
        QFileInfo fileInfo(s);
        ui->label_file->setText("{"+fileInfo.suffix()+"}  "+fileInfo.fileName());
    }
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
