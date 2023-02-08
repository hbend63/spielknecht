#include "part.h"

Part::Part(QObject *parent)
    : QObject{parent}
{
   mProcessState=NEW;
}

QString Part::uID() const
{
    return mUID;
}

void Part::setUID(const QString &newUID)
{
    mUID = newUID;
}

QTime Part::processStart() const
{
    return mProcessStart;
}

void Part::setProcessStart()
{
    if (mProcessState==NEW)
    {
       mProcessStart = QTime::currentTime();
       mProcessState=RUNNING;
    }
}

QTime Part::processReady() const
{
    return mProcessReady;
}

void Part::setProcessReady()
{   if (mProcessState==RUNNING)
    {
      mProcessReady = QTime::currentTime();
      mProcessState=READY;
    }
}

void Part::setProcessDone()
{
    if (mProcessState==READY)
          mProcessState=DONE;

}

void Part::cancelProcess()
{
    if (mProcessState!=NEW)
    {
      mProcessReady = QTime::currentTime();
      mProcessState=CANCELLED;
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
