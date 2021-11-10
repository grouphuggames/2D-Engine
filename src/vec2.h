#pragma once

#include "Core.h"

struct vec2
{
public:
  f32 data[2];

  vec2()
  {
    data[0] = 0.f;
    data[1] = 0.f;
  }
  
  vec2(f32 x, f32 y)
  {
    data[0] = x;
    data[1] = y;
  }

  f32 x() const
  {
    return data[0];
  }

  f32 y() const
  {
    return data[1];
  }
};

void DebugPrintVec2(vec2 vec)
{
  DebugPrintToConsole("{ ", vec.x(), ", ", vec.y(), " }");
}