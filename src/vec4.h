#pragma once

#include "Core.h"

struct vec4
{
public:
  f32 data[4];
  
  vec4()
  {
    data[0] = 0.f;
    data[1] = 0.f;
    data[2] = 0.f;
    data[3] = 0.f;
  }

  vec4(f32 val)
  {
    data[0] = val;
    data[1] = val;
    data[2] = val;
    data[3] = val;
  }
  
  vec4(f32 x, f32 y, f32 z, f32 w)
  {
    data[0] = x;
    data[1] = y;
    data[2] = z;
    data[3] = w;
  }

  f32 x() const
  {
    return data[0];
  }

  f32 y() const
  {
    return data[1];
  }

  f32 z() const
  {
    return data[2];
  }

  f32 w() const
  {
    return data[3];
  }
};

void DebugPrintVec4(vec4 vec)
{
  DebugPrintToConsole("{ ", vec.x(), ", ", vec.y(), ", ", vec.z(), ", ", vec.w(), " }");
}