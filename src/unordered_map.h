#pragma once

#include "Core.h"

namespace en
{
  template <typename T1, typename T2>
  class unordered_map
  {
    s32 size = 0;
    s32 find_index = -1;

  public:
    en::vector<T1> keys;
    en::vector<T2> values;

    unordered_map<T1, T2>() {}

    void Insert(T1 key, T2 val)
    {
      keys.PushBack(key);
      values.PushBack(val);
      size++;
    }

    bool Find(T1 key)
    {
      bool found = false;

      for (s32 i = 0; i < size; i++)
      {
        if (key == keys[i])
        {
          find_index = i;
          found = true;
          break;
        }
      }

      return found;
    }

    T2 At(T1 key)
    {
      if (Find(key))
      {
        return values[find_index];
      }

      return T2();
    }

    s32 Size() const
    {
      return size;
    }

    void Clear()
    {
      keys.Clear();
      values.Clear();

      size = 0;
      find_index = -1;
    }
  };
}