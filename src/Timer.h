#pragma once

#include <chrono>

enum class TIME
{
  NANOSECOND,
  MICROSECOND,
  MILLISECOND
};

struct TimerInfo
{
  TIME time_scale = TIME::MILLISECOND;
  std::chrono::time_point<std::chrono::steady_clock> timer_start;
  std::chrono::time_point<std::chrono::steady_clock> timer_stop;
  u32 time_delta;
};

void StartTimer(TimerInfo& info)
{
  info.timer_start = std::chrono::high_resolution_clock::now();
}

u32 GetTimerValue(TimerInfo& info)
{
  auto current_time = std::chrono::high_resolution_clock::now();
  if (info.time_scale == TIME::NANOSECOND)
    return (u32)std::chrono::duration_cast<std::chrono::nanoseconds>(current_time - info.timer_start).count();
  else if (info.time_scale == TIME::MICROSECOND)
    return (u32)std::chrono::duration_cast<std::chrono::microseconds>(current_time - info.timer_start).count();
  else if (info.time_scale == TIME::MILLISECOND)
    return (u32)std::chrono::duration_cast<std::chrono::milliseconds>(current_time - info.timer_start).count();

  return 0;
}

void StopTimer(TimerInfo& info)
{
  info.timer_stop = std::chrono::high_resolution_clock::now();
  if (info.time_scale == TIME::NANOSECOND)
    info.time_delta = (u32)std::chrono::duration_cast<std::chrono::nanoseconds>(info.timer_stop - info.timer_start).count();
  else if (info.time_scale == TIME::MICROSECOND)
    info.time_delta = (u32)std::chrono::duration_cast<std::chrono::microseconds>(info.timer_stop - info.timer_start).count();
  else if (info.time_scale == TIME::MILLISECOND)
    info.time_delta = (u32)std::chrono::duration_cast<std::chrono::milliseconds>(info.timer_stop - info.timer_start).count();
}