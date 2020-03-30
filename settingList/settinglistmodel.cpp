#include "settinglistmodel.h"
//#include "SystemSetting/syssettingwidget.h"
SettingListModel::SettingListModel(QObject *parent) :
    QAbstractListModel(parent)
{
    mListdata = new  QList<SettingData> ();
}

SettingListModel::SettingListModel(QList<SettingData> &list,QObject *parent)
{
    Q_UNUSED(parent);
    mListdata = new  QList<SettingData>(list);
}

SettingListModel::~SettingListModel()
{
    delete mListdata;
}

int SettingListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mListdata->count();
}

QVariant SettingListModel::data(const QModelIndex &index, int role) const
{

    int pos = index.row();
    if(!index.isValid() || pos > mListdata->count() )
    {
        return QVariant();
    }
    if(role==Qt::DisplayRole)//文字
    {
        return mListdata->at(pos).m_name;
    }
    else if(role==Qt::DecorationRole)
    {
        return mListdata->at(pos).mIcon;
    }
    else if(role==Qt::UserRole+4)
    {
//        if(mListdata->at(0).m_name == "English") //翻译未完成
//        {
//            if((SysSettingWidget::isNotCompleteLanguage(pos)==true))
//            {
//                return 1;
//            }
//            else
//            {
//                return 0;
//            }
//            return 0;
//        }
//        else
//        {
//            return 0;
//        }
    }
    else
    {
        return QVariant();
    }

    return QVariant();
}
