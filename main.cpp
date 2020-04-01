#include "mainwindow.h"
#include "common.h"
#include "global/globalvar.h"
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GlobalVar::APPLICATION_DIR_PATH = QCoreApplication::applicationDirPath();
    GlobalVar::IMG_SAVE_DIR_PATH = GlobalVar::APPLICATION_DIR_PATH+IMG_SAVE_DIR;
    QDir dir;
    if(!dir.exists(GlobalVar::IMG_SAVE_DIR_PATH))
    {
        dir.mkpath(GlobalVar::IMG_SAVE_DIR_PATH);
    }
    MainWindow w;
    w.show();

    return a.exec();
}
