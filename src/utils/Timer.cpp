#include "Timer.h"
// static
SystemTime Timer::GetCurrentTime()
{
  return std::chrono::system_clock::now();
}

SystemDurationMs Timer::FindTimeDifference(SystemTime timeStart, SystemTime timeEnd)
{
  return timeEnd - timeStart;
}

float Timer::GetCurrentTimeMs()
{
  return GetCurrentTime().time_since_epoch().count();
}

float Timer::FindTimeDifferenceFloat(SystemTime timeStart, SystemTime timeEnd)
{
  return (timeEnd - timeStart).count();
}

// non static
Timer::Timer(TimeType type)
  : timeType(type),
  _mTimeStart(GetCurrentTime()),
  _mTotalTimeElapsed(0),
  _mIsTimerActive(false),
  _mIsTimerStarted(false)
{}

Timer::~Timer()
{}

// start the timer - will not do anything if timer is started already
void Timer::startTimer()
{
  if (_mIsTimerStarted)
  {
    // DO NOTHING IF TIMER IS ALREADY STARTED
    return;
  }

  _mIsTimerActive = true;
  _mIsTimerStarted = true;
  _mTimeStart = GetCurrentTime();
  _mTotalTimeElapsed = SystemDurationMs(0);
}

// pause the timer
float Timer::pauseTimer()
{
  if (!_mIsTimerStarted || !_mIsTimerActive) 
    return getTimeElapsed();

  float timeElapsed = getTimeElapsed();
  _mTotalTimeElapsed += GetCurrentTime() - _mTimeStart;
  _mIsTimerActive = false;
  return timeElapsed;
}

// resume the timer
float Timer::resumeTimer()
{
  if (!_mIsTimerStarted || _mIsTimerActive) 
    return getTimeElapsed();

  float timeElapsed = getTimeElapsed();
  _mTimeStart = GetCurrentTime();
  _mIsTimerActive = true;
  return timeElapsed;
}

// stop timer and reset to 0
float Timer::stopTimer()
{
  if (!_mIsTimerStarted)
    return getTimeElapsed();

  float timeElapsed = getTimeElapsed();
  _mTotalTimeElapsed = SystemDurationMs(0);
  _mTimeStart = GetCurrentTime();
  _mIsTimerActive = false;
  _mIsTimerStarted = false;

  return timeElapsed;
}

// get time elapsed since timer is started
float Timer::getTimeElapsed() const
{
  if (!_mIsTimerStarted) return 0;

  if (_mIsTimerActive) {
    SystemDurationMs elapsed = GetCurrentTime() - _mTimeStart;
    auto totalElapsed = _mTotalTimeElapsed + elapsed;
    return totalElapsed.count() / float(timeType);
  }
  else return _mTotalTimeElapsed.count() / float(timeType);
}

bool Timer::isTimerActive() const
{
  return _mIsTimerActive;
}

bool Timer::isTimerStarted() const
{
  return _mIsTimerStarted;
}