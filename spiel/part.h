#ifndef PART_H
#define PART_H

#include <QObject>
#include <QTime>

enum state {NEW,RUNNING,DONE};

class Part : public QObject
{
    Q_OBJECT
public:
    explicit Part(QObject *parent = nullptr);

    QString uID() const;
    void setUID(const QString &newUID);

    QTime startTime() const;
    QTime endTime() const;

    void startProcess();

    void stepNext();
    void stepPrior();

    state processState() const;
    state lastState() const;

    int timerStart() const;
    void setTimerStart(int newTimerStart);

    int timerEnd() const;
    void setTimerEnd(int newTimerEnd);

signals:

private:
    QString mUID;
    QTime mProcessStart, mProcessEnd;
    int mTimerStart{0}, mTimerEnd{0};
    state mProcessState{NEW}, mLastState{NEW};

};

#endif // PART_H
