#include "globalvar.h"
#include "common.h"
#include <QDebug>

const QString GlobalVar::VERSION_INTERNAL = "1.03.03";
QString GlobalVar::APPLICATION_DIR_PATH;
QString GlobalVar::IMG_SAVE_DIR_PATH;

GlobalVar *GlobalVar::mGlobalVar=NULL;

GlobalVar::GlobalVar(QObject *parent) :
    QObject(parent)
{
}

GlobalVar *GlobalVar::getObj()
{
    if(mGlobalVar==NULL)
        mGlobalVar=new GlobalVar();
    return mGlobalVar;
}
