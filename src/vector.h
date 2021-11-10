#pragma once

#include "Core.h"

namespace en
{
  template <typename T>
  class vector
  {
    T* data;
    s32 capacity;
    s32 size;

  public:
    vector<T>() : capacity(10), size(0) { data = new T[capacity]; }
    vector<T>(s32 _size) : capacity(_size + 1), size(0) { data = new T[capacity]; }

    T* begin() { return &data[0]; }
    T* end() { return &data[size]; }

    T& operator[](s32 index) { return data[index]; }

    void Resize(s32 _size)
    {
      T* new_arr = new T[_size];

      for (s32 i = 0; i < size; ++i) { new_arr[i] = data[i]; }
      capacity = _size;

      delete[] data;
      data = nullptr;
      data = new_arr;
    }

    void PushBack(T val)
    {
      if (size >= capacity) Resize(2 * capacity);

      data[size] = val;
      ++size;
    }

    void Clear()
    {
      en::vector<T> tmp_vec;
      *this = tmp_vec;
    }

    s32 Size() const
    {
      return size;
    }
  };
}
