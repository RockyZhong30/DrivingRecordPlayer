#ifndef GLOBALVAR_H
#define GLOBALVAR_H

#include <QObject>
#include <QPixmap>
#include <QApplication>

class GlobalVar : public QObject
{
public:
    const static QString VERSION_INTERNAL;
    static QString APPLICATION_DIR_PATH;
    static QString IMG_SAVE_DIR_PATH;
    static GlobalVar *getObj();

private:
    explicit GlobalVar(QObject *parent = 0);
    static GlobalVar *mGlobalVar;
};
#endif //GLOBALVAR_H
