#pragma once
#include <chrono>

typedef std::chrono::time_point<std::chrono::system_clock> SystemTime;
typedef std::chrono::duration<double, std::milli> SystemDurationMs;

class Timer
{
private:
  SystemTime _mTimeStart;
  SystemDurationMs _mTotalTimeElapsed;
  bool _mIsTimerActive;
  bool _mIsTimerStarted;

public:
  static SystemTime GetCurrentTime();
  static float GetCurrentTimeMs();
  static SystemDurationMs FindTimeDifference(SystemTime timeStart, SystemTime timeEnd);
  static float FindTimeDifferenceFloat(SystemTime timeStart, SystemTime timeEnd);

  // timer output type
  enum class TimeType {
    millisecond = 1,
    second = 1000,
    minute = 60 * 1000,
    hour = 60 * 60 * 1000
  };

  Timer(TimeType type = TimeType::millisecond);
  virtual ~Timer();
  
  // start the timer
  void startTimer();

  // pause the timer
  float pauseTimer();

  // resume the timer
  float resumeTimer();
  
  // stop timer and reset start time
  float stopTimer();

  // get time elapsed since timer is started
  float getTimeElapsed() const;

  bool isTimerStarted() const;
  bool isTimerActive() const;

  // time type returned by the instance, millisecond by default
  TimeType timeType;
};