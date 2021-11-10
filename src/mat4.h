#pragma once

#include "Core.h"
#include "vec3.h"

class mat4
{
public:
  f32 elements[16] = {};

  mat4() {}

  static mat4 Identity()
  {
    mat4 result;

    result.elements[0] = 1.f;
    result.elements[5] = 1.f;
    result.elements[10] = 1.f;
    result.elements[15] = 1.f;

    return result;
  }

  static mat4 Translate(const vec3& translation)
  {
    mat4 result = Identity();

    result.elements[12] = translation.x();
    result.elements[13] = translation.y();
    result.elements[14] = translation.z();

    return result;
  }

  static mat4 Rotate(f32 angle, const vec3& axis)
  {
    f32 sine = sinf(angle);
    f32 cosine = cosf(angle);
    f32 one_minus_cosine = 1.f - cosine;
    vec3 norm_axis = vec3::Normalize(axis);

    mat4 result = mat4::Identity();

    result.elements[0] = cosine + norm_axis.x() * norm_axis.x() * one_minus_cosine;
    result.elements[1] = norm_axis.x() * norm_axis.y() * one_minus_cosine + norm_axis.z() * sine;
    result.elements[2] = norm_axis.x() * norm_axis.z() * one_minus_cosine - norm_axis.y() * sine;
    result.elements[4] = norm_axis.y() * norm_axis.x() * one_minus_cosine - norm_axis.z() * sine;
    result.elements[5] = cosine + norm_axis.y() * norm_axis.y() * one_minus_cosine;
    result.elements[6] = norm_axis.y() * norm_axis.z() * one_minus_cosine + norm_axis.x() * sine;
    result.elements[8] = norm_axis.z() * norm_axis.x() * one_minus_cosine + norm_axis.y() * sine;
    result.elements[9] = norm_axis.z() * norm_axis.y() * one_minus_cosine - norm_axis.x() * sine;
    result.elements[10] = cosine + norm_axis.z() * norm_axis.z() * one_minus_cosine;
    result.elements[15] = 1.f;

    return result;
  }

  static mat4 Scale(const vec3& scale)
  {
    mat4 result = Identity();

    result.elements[0] = scale.x();
    result.elements[5] = scale.y();
    result.elements[10] = scale.z();

    return result;
  }

  friend mat4 operator*(mat4 left, const mat4& right)
  {
    mat4 result;

    for (s32 y = 0; y < 4; y++)
    {
      for (s32 x = 0; x < 4; x++)
      {
	f32 sum = 0.f;

	for (s32 e = 0; e < 4; e++)
	{
	  sum += left.elements[x + e * 4] * right.elements[e + y * 4];
	}

	result.elements[x + y * 4] = sum;
      }
    }

    return result;
  }

  mat4& operator*=(const mat4& other)
  {
    *this = *this * other;
    return *this;
  }
};