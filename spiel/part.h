#ifndef PART_H
#define PART_H

#include <QObject>
#include <QTime>

enum state {NEW, RUNNING,READY,DONE,CANCELLED};

class Part : public QObject
{
    Q_OBJECT
public:
    explicit Part(QObject *parent = nullptr);

    QString uID() const;
    void setUID(const QString &newUID);

    QTime processStart() const;
    void setProcessStart();

    QTime processReady() const;
    void setProcessReady();

    void setProcessDone();


    void cancelProcess();

    state processState() const;

    int timerStart() const;
    void setTimerStart(int newTimerStart);

    int timerEnd() const;
    void setTimerEnd(int newTimerEnd);

signals:
private:
    QString mUID;
    QTime mProcessStart, mProcessReady;
    int mTimerStart{0}, mTimerEnd{0};
    state mProcessState;

};

#endif // PART_H
