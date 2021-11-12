#pragma once

#include "Core.h"
#include "Entity.h"
#include "unordered_map.h"
#include "glfw/glfw3.h"
#include "Scene.h"


enum class BUTTON_ACTION
{
  PRESS,
  HOLD
};

// the user input system currently supports functions that return void with no params and
// functions that require a GLFWwindow* passed to them and return void

// lambdas that do not capture or require params are also supported

// currently data can only be modified if it is in global scope

using InputFunc = void(*)(void);
using GLFWInputFunc = void(*)(GLFWwindow*);   // this is used ONLY when setting up input for a function
                                              // that requires a GLFWwindow* to be passed to it

struct FuncInfo
{
  InputFunc func;
  BUTTON_ACTION action;
};

static struct
{
  en::unordered_map<s32, FuncInfo> _InputFuncs = {};
} input_wrapper;

void SetKeyboardInput(InputFunc func, s32 button, BUTTON_ACTION action)
{
  FuncInfo f;
  f.func = func;
  f.action = action;

  if (input_wrapper._InputFuncs.Find(button) == false)
  {
    input_wrapper._InputFuncs.Insert(button, f);
  }
  else
  {
    input_wrapper._InputFuncs.At(button) = f;
  }
}

s32 possible_keybindings[] = 
{
  GLFW_KEY_SPACE,
  GLFW_KEY_A,
  GLFW_KEY_D,
  GLFW_KEY_S,
  GLFW_KEY_W,
  GLFW_KEY_ESCAPE
};

void ProcessKeyboardInput(GLFWwindow* window)
{
  for (s32 i = 0; i < (sizeof(possible_keybindings) / sizeof(possible_keybindings[0])); i++)
  {
    if (glfwGetKey(window, possible_keybindings[i]) == GLFW_PRESS && input_wrapper._InputFuncs.At(possible_keybindings[i]).action == BUTTON_ACTION::HOLD)
    {
        if (!input_wrapper._InputFuncs.At(possible_keybindings[i]).func)
        {
            DebugPrintToConsole("Empty function!");
        }
        else
        {
            input_wrapper._InputFuncs.At(possible_keybindings[i]).func();
        }
    }
  }
}

void KeyCallback(GLFWwindow* window, s32 key, s32 scancode, s32 action, s32 mods)
{
  for (s32 i = 0; i < (sizeof(possible_keybindings) / sizeof(possible_keybindings[0])); i++)
  {
    if (key == possible_keybindings[i] && action == GLFW_PRESS)
    {
      if (input_wrapper._InputFuncs.At(key).action == BUTTON_ACTION::PRESS)
      {
        try
        {
          input_wrapper._InputFuncs.At(key).func();
        }
        catch (const std::exception&)
        {
            return;
        }
      }
    }
  }
}

void MouseButtonCallback(GLFWwindow* window, s32 button, s32 action, s32 mods)
{
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
  {
    f64 mouse_x, mouse_y;
    glfwGetCursorPos(window, &mouse_x, &mouse_y);

    mouse_x = 2.0 * (mouse_x / (f64)window_width) - 1.0;
    mouse_y = 2.0 * (1.0 - (mouse_y / (f64)window_height)) - 1.0;
  }
}

void FramebufferSizeCallback(GLFWwindow* window, s32 width, s32 height)
{
  window_width = width;
  window_height = height;
  aspect_ratio = (f32)window_width / (f32)window_height;

  for (s32 i = 0; i < entities.Size(); i++)
  {
    entities[i].scale = vec2(original_scales[i].x(), original_scales[i].y() * aspect_ratio);
  }

  glViewport(0, 0, width, height);
}
