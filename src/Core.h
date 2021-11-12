#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <cstring>
#include <cmath>
#include <filesystem>
#include <random>


using s32 = int32_t;
using u32 = uint32_t;
using f32  = float;
using f64 = double;

s32 window_width = 1280;
s32 window_height = 720;
f32 aspect_ratio = 0.f;

static void DebugPrintToConsole() {}

template<typename T, typename... Types>
static void DebugPrintToConsole(T var1, Types... var2)
{
  std::cout << var1;
  DebugPrintToConsole(var2...);
  std::cout << '\n';
}

std::string AssetPath(const char* filename)
{
  std::string prefix = "../../assets/";
  return prefix.append(filename);
}

std::string AssetPath(std::string path)
{
  std::string prefix = "../../assets/";
  return prefix.append(path);
}
