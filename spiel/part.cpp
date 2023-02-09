#include "part.h"

Part::Part(QObject *parent)
    : QObject{parent}
{

}

QString Part::uID() const
{
    return mUID;
}

void Part::setUID(const QString &newUID)
{
    mUID = newUID;
}

QTime Part::startTime() const
{
    return mProcessStart;
}

void Part::startProcess()
{
    if (mProcessState==NEW)
    {
       mProcessStart = QTime::currentTime();
       mProcessState=RUNNING;
    }
}

void Part::stepNext()
{
    int st=static_cast<int>(mProcessState);
    if (mProcessState <= DONE)
    {   st++;
        mLastState=mProcessState;
        mProcessState = static_cast<state>(st);
        if ((mLastState==RUNNING) && (mProcessState==DONE))
           mProcessEnd = QTime::currentTime();
    }
}

QTime Part::endTime() const
{
    return mProcessEnd;
}


void Part::stepPrior()
{
    int st=static_cast<int>(mProcessState);
    if (mProcessState > NEW)
    {
        st--;
        mProcessState = static_cast<state>(st);
    }
}

state Part::processState() const
{
    return mProcessState;
}

int Part::timerStart() const
{
    return mTimerStart;
}

void Part::setTimerStart(int newTimerStart)
{
    mTimerStart = newTimerStart;
}

int Part::timerEnd() const
{
    return mTimerEnd;
}

void Part::setTimerEnd(int newTimerEnd)
{
    mTimerEnd = newTimerEnd;
}

state Part::lastState() const
{
    return mLastState;
}
