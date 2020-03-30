#ifndef SETTINGLISTMODEL_H
#define SETTINGLISTMODEL_H
#include <QAbstractListModel>
#include <QLayout>
//#include "globalvar.h"
class SettingData
{
public:
    QString m_name;
    QString mIcon;
    int mType;
    QWidget *mWidget;
    SettingData(const QString &str1,const QString &str2 = QString(), int type = 0)
    {
        m_name = str1;
        mIcon = str2;
        mType = type;
    }
//    SettingData(char *str1,char *str2 = NULL,int type = 0)
//    {
//        m_name = str1;
//        mType = type;
//        if(str2!=NULL)
//        {
//            mIcon = str2;
//        }
//        else
//        {
//            mIcon = QString();
//        }
//    }
};

class SettingListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit SettingListModel(QObject *parent = 0);
    SettingListModel(QList<SettingData> &list,QObject *parent=0);
    ~SettingListModel();
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

private:
    QList<SettingData> *mListdata;
};

#endif // SETTINGLISTMODEL_H
