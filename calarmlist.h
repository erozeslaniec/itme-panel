#ifndef CALARMLIST_H
#define CALARMLIST_H

#include <QAbstractListModel>
#include <QStringList>
#include <QFile>

class Alarm{
  public:
    Alarm(const QString &pdescription, const QString &poccured,const int pAlarmId, const QString &prestore="Active" );
    QString Description() const;
    QString Occurrence() const;
    QString Restore() const;
    bool Active() const;
    int AlarmID() const;

    void setActive();
    void setInactive();
    void setRestore(QString prestore);

private:
QString description;
QString occurrence;
QString restore;


bool active;
int alarmId;

};

class CAlarmList : public QAbstractListModel
{
    Q_OBJECT
public:

    enum AlarmRoles{
        DescriptionRole=Qt::UserRole+1,
        OccurrenceRole,
        RestoreRole,
        ActiveRole
    };



    CAlarmList(QObject *parent = nullptr);
    void addAlarm(const Alarm &alarm);
    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
    bool Active(int index);
    void setInactive(int index);
    void setRestore(int index,QString restore);
    int AlarmID(int index);


    void readAlarmFile();
    void writeAlarmFile();



signals:
    void dataChanged();
    void modelReset();

public slots:

protected:
    QHash<int, QByteArray> roleNames() const;
private:
    QList<Alarm> mAlarms;
    QString filename;
};

#endif // CALARMLIST_H
