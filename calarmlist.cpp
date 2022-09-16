#include "calarmlist.h"

#include <QTextStream>

CAlarmList::CAlarmList(QObject *parent):QAbstractListModel(parent)
{
    filename="alarmLog.csv";
    readAlarmFile();

}

void CAlarmList::addAlarm(const Alarm &alarm)
{
    emit beginInsertRows(QModelIndex(),rowCount(),rowCount());
    mAlarms.insert(0,alarm);
    emit endInsertRows();

    writeAlarmFile();


}

int CAlarmList::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mAlarms.count();
}

QVariant CAlarmList::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mAlarms.count())
        return QVariant();

    const Alarm &alarm = mAlarms[index.row()];
    if(role==DescriptionRole)
        return alarm.Description();
    else if (role==OccurrenceRole)
        return alarm.Occurrence();
    else if (role==RestoreRole)
        return alarm.Restore();
    else if (role==ActiveRole)
        return alarm.Active();

    return QVariant();
}

bool CAlarmList::Active(int index)
{
    if (index < 0 || index >= mAlarms.count())return false;
    return mAlarms[index].Active();
}

void CAlarmList::setInactive(int index)
{
    if (index < 0 || index >= mAlarms.count())return;
    mAlarms[index].setInactive();


}

void CAlarmList::setRestore(int index, QString restore)
{
    if (index < 0 || index >= mAlarms.count())return;
    mAlarms[index].setRestore(restore);

}

int CAlarmList::AlarmID(int index)
{
    if (index < 0 || index >= mAlarms.count())return 0;
    return mAlarms[index].AlarmID();

}

void CAlarmList::readAlarmFile()
{
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        mAlarms.clear();
        while(!file.atEnd()){

            QByteArray line=file.readLine();
            if(line.split(';').size()==4){
                Alarm alarm(line.split(';').at(0),
                            line.split(';').at(1),
                            line.split(';').at(2).toInt(),
                            line.split(';').at(3)
                            );

                alarm.setInactive();
                mAlarms.push_back(alarm);}
        }
        file.close();}
}

void CAlarmList::writeAlarmFile()
{
    QFile file(filename);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    for(int i=0;i<mAlarms.size();i++)
        out<<mAlarms[i].Description()<<";"<<mAlarms[i].Occurrence()<<";"<<mAlarms[i].AlarmID()<<";"<<mAlarms[i].Restore()<<"\n";

    file.close();

}


QHash<int, QByteArray> CAlarmList::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[DescriptionRole]="description";
    roles[OccurrenceRole]="occurrence";
    roles[RestoreRole]="restore";
    roles[ActiveRole]="active";
    return roles;
}

Alarm::Alarm(const QString &pdescription, const QString &poccured, const int pAlarmId, const QString &prestore)
    :description(pdescription),occurrence(poccured),restore(prestore),active(true),alarmId(pAlarmId)
{
}

QString Alarm::Description() const
{
    return description;
}

QString Alarm::Occurrence() const
{
    return occurrence;
}

QString Alarm::Restore() const
{
    return restore;
}

bool Alarm::Active() const
{
    return active;
}

int Alarm::AlarmID() const
{
    return alarmId;
}

void Alarm::setActive()
{
    active=true;
}

void Alarm::setInactive(){
    active=false;
}

void Alarm::setRestore(QString prestore)
{
    restore=prestore;
}


