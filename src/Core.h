#pragma once

#include <iostream>

using s32 = int32_t;
using u32 = uint32_t;
using f32  = float;
using f64 = double;

static void DebugPrintToConsole() {}

template<typename T, typename... Types>
static void DebugPrintToConsole(T var1, Types... var2)
{
  std::cout << var1;
  DebugPrintToConsole(var2...);
  std::cout << '\n';
}
