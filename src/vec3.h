#pragma once

#include "Core.h"

struct vec3
{
  f32 data[3];

  vec3()
  {
    data[0] = 0.f;
    data[1] = 0.f;
    data[2] = 0.f;
  }

  vec3(f32 val)
  {
    data[0] = val;
    data[1] = val;
    data[2] = val;
  }
  
  vec3(f32 x, f32 y, f32 z)
  {
    data[0] = x;
    data[1] = y;
    data[2] = z;
  }

  vec3(const vec2 xy, f32 z)
  {
    data[0] = xy.x();
    data[1] = xy.y();
    data[2] = z;
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

  static f32 Length(const vec3& vec)
  {
    return sqrtf(vec.x() * vec.x() + vec.y() * vec.y() + vec.z() * vec.z());
  }

  static vec3 Normalize(const vec3& vec)
  {
    f32 k = Length(vec);
    return vec3(vec.x() / k, vec.y() / k, vec.z() / k);
  }
};

void DebugPrintVec3(vec3 vec)
{
  DebugPrintToConsole("{ ", vec.x(), ", ", vec.y(), ", ", vec.z(), " }");
}