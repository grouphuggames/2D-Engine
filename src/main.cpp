#define GLEW_STATIC

#include <iostream>
#include "glew/glew.h"
#include "glfw/glfw3.h"
#include <cstdint>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cmath>
#include <string>
#include <chrono>
#include <filesystem>
#include <random>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"
#include "fmod/fmod.hpp"
#include "stb/stb_image.h"


using s32 = int32_t;
using u32 = uint32_t;
using f32  = float;
using f64 = double;


s32 window_width = 1280;
s32 window_height = 720;
f32 aspect_ratio = 0.f;

const f64 PI = 3.14159;

static inline f32 ToRadians(f32 degrees)
{
  return degrees * ((f32)PI / 180.f);
}

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

/// Audio API Reference
////// bool InitSound();
////// bool LoadSound(const char* path);
////// bool PlaySound(const char* path);
////// bool StopSound(const char* path);

FMOD::System* audio_system;
FMOD::Channel* audio_channels[4];   // audio system can play up to 4 sounds simultaneously

en::unordered_map<std::string, FMOD::Sound*> scene_sounds;

bool InitSound()
{
  if (FMOD::System_Create(&audio_system) != FMOD_OK) return false;
  if (audio_system->init(64, FMOD_INIT_NORMAL, 0)) return false;

  return true;
}

FMOD::Sound* LoadSound(const char* path, bool looping)
{
  FMOD::Sound* sound;
  std::string sound_path = AssetPath("audio/").append(path);
  if (looping)
  {
    if (audio_system->createSound(sound_path.c_str(), FMOD_DEFAULT | FMOD_LOOP_NORMAL, 0, &sound) == FMOD_OK)
    {
      DebugPrintToConsole("Successfully loaded audio asset: ", sound_path);
      return sound;
    }
  }
  else
  {
    if (audio_system->createSound(sound_path.c_str(), FMOD_DEFAULT, 0, &sound) == FMOD_OK)
    {
      DebugPrintToConsole("Successfully loaded audio asset: ", sound_path);
      return sound;
    }
  }

  return nullptr;
}

int audio_channels_in_use = 0;

bool PlaySound(const char* sound)
{
  std::string sound_path = AssetPath("audio/").append(sound);
  if (audio_system->playSound(scene_sounds.At(sound), nullptr, false, &audio_channels[audio_channels_in_use++]) == FMOD_OK)
  {
    return true;
  }

  DebugPrintToConsole("Failed to play sound!");
  return false;
}

bool StopSound(const char* sound)           // TODO : write this function!
{
  return false;
}

/// Graphics API Reference
////// u32 LoadGLTexture(const char* texture);
////// u32 LoadGLShader(const char* shader);

en::unordered_map<std::string, u32> scene_textures;

u32 LoadGLTexture(const char* texture)
{
  std::string filepath = AssetPath("textures/").append(texture);

  u32 texture_id;
  glGenTextures(1, &texture_id);

  s32 width, height, nr_components;
  unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &nr_components, 0);
  if (data)
  {
    GLenum format;
    if (nr_components == 1)  format = GL_RED;
    else if (nr_components == 3)  format = GL_RGB;
    else if (nr_components == 4)  format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    DebugPrintToConsole("Successfully loaded texture: ", filepath);
  }
  else
  {
    DebugPrintToConsole("Texture failed to load: ", filepath);
    stbi_image_free(data);
  }

  return texture_id;
}

enum class SHADER_TYPE
{
  NONE = -1,
  VERTEX,
  FRAGMENT
};

en::unordered_map<std::string, u32> scene_shaders;

u32 LoadGLShader(const char* shader)
{
  std::string filepath = AssetPath("shaders/").append(shader);
  std::ifstream stream(filepath);
  std::string line;
  std::stringstream shader_streams[2];
  SHADER_TYPE type = SHADER_TYPE::NONE;

  while (std::getline(stream, line))
  {
    if (line.find("#shader") != std::string::npos)
    {
      if (line.find("vertex") != std::string::npos)
      {
        type = SHADER_TYPE::VERTEX;
      }
      else if (line.find("fragment") != std::string::npos)
      {
        type = SHADER_TYPE::FRAGMENT;
      }
      else
      {
        char error_type[128] = "Unsupported shader type: ";
        strcat(error_type, line.c_str());
        DebugPrintToConsole("Failed to load shader: ", filepath);
        DebugPrintToConsole(error_type);
      }
    }
    else
    {
      shader_streams[(s32)type] << line << '\n';
    }
  }

  stream.close();

  u32 vs = glCreateShader(GL_VERTEX_SHADER);
  std::string vs_source_data = shader_streams[(s32)SHADER_TYPE::VERTEX].str();
  const char* vs_source = vs_source_data.c_str();
  glShaderSource(vs, 1, &vs_source, nullptr);
  glCompileShader(vs);
  s32 success = 0;
  char log_info[512];
  glGetShaderiv(vs, GL_COMPILE_STATUS, &success);

  if (!success)
  {
    glGetShaderInfoLog(vs, 512, nullptr, log_info);
    DebugPrintToConsole("Failed to load shader: ", filepath);
    DebugPrintToConsole(log_info);

    return 9999;
  }

  u32 fs = glCreateShader(GL_FRAGMENT_SHADER);
  std::string fs_source_data = shader_streams[(s32)SHADER_TYPE::FRAGMENT].str();
  const char* fs_source = fs_source_data.c_str();
  glShaderSource(fs, 1, &fs_source, nullptr);
  glCompileShader(fs);
  glGetShaderiv(fs, GL_COMPILE_STATUS, &success);

  if (!success)
  {
    glGetShaderInfoLog(fs, 512, nullptr, log_info);
    DebugPrintToConsole("Failed to load shader: ", filepath);
    DebugPrintToConsole(log_info);

    return 9999;
  }

  u32 program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &success);

  if (!success)
  {
    glGetProgramInfoLog(program, 512, nullptr, log_info);
    DebugPrintToConsole("Failed to load shader: ", filepath);
    DebugPrintToConsole(log_info);

    return 9999;
  }

  glUseProgram(program);
  glDeleteShader(vs);
  glDeleteShader(fs);
  shader_streams[0].str("");
  shader_streams[1].str("");

  DebugPrintToConsole("Successfully loaded shader: ", filepath);
  
  return program;
}

f32 GetRandomFloat()
{
  std::mt19937 generator((u32)std::chrono::steady_clock::now().time_since_epoch().count());
  std::uniform_real_distribution<f32> distribution(0.f, 1.f);
  return distribution(generator);
}

f32 GetRandomFloatInRange(f32 lower, f32 upper)
{
  std::mt19937 generator((u32)std::chrono::steady_clock::now().time_since_epoch().count());
  std::uniform_real_distribution<f32> distribution(lower, upper);
  return distribution(generator);
}

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

TimerInfo game_timer;
TimerInfo frame_timer;
f32 game_time = 0.f;

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

struct Vertex
{
  vec2 position;
};

struct ParticleVertex
{
  vec2 position;
  vec4 color;
  vec2 texture_coords;
  f32 texture_index;
};

u32 quad_indices[] = { 0, 1, 3, 1, 2, 3 };

struct Entity
{
public:
  Vertex verts[4] = { vec2(1.f, 1.f), vec2(1.f, -1.f), vec2(-1.f, -1.f), vec2(-1.f, 1.f) };
  vec2 position;
  vec2 scale;
  u32 vao, vbo, ebo;
  u32 shader;
  u32 texture;
  f32 angle = 0.f;
  bool editor_selected = false;
};

f32 delta_time = 0.f;
f32 last_frame = 0.f;

en::vector<Entity> entities;
en::vector<vec2> original_scales;

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

en::vector<std::string> LoadSceneSelector()
{
  en::vector<std::string> result;
  
  for (const auto& file : std::filesystem::directory_iterator(AssetPath("")))
  {
    std::string filepath = file.path().string();
    
    if (filepath.find(".enscene") != std::string::npos)
    {
      result.PushBack(filepath);
    }
  }

  return result;
}

void LoadScene(const char* scene_path)
{
  entities.Clear();
  original_scales.Clear();
  scene_shaders.Clear();
  scene_textures.Clear();
  scene_sounds.Clear();

  std::string path = AssetPath("scenes/").append(scene_path);
  std::ifstream scene_stream(path);

  std::string line;

  en::vector<std::string> scene_shader_names;
  en::vector<std::string> scene_texture_names;
  en::vector<std::string> scene_sound_names;

  while (std::getline(scene_stream, line))
  {
    if (line.find(".glsl") != std::string::npos)
    {
      scene_shader_names.PushBack(line);
    }
    else if (line.find(".png") != std::string::npos)
    {
      scene_texture_names.PushBack(line);
    }
    else if (line.find(".wav") != std::string::npos)
    {
      scene_sound_names.PushBack(line);
    }
    else
    {
      break;
    }
  }

  for (const auto& s : scene_shader_names)
  {
    std::string s_path = s;
    u32 shader = LoadGLShader(s_path.c_str());
    scene_shaders.Insert(s_path, shader);
  }

  for (const auto& t : scene_texture_names)
  {
    std::string t_path = t;
    u32 texture = LoadGLTexture(t_path.c_str());
    scene_textures.Insert(t_path, texture);
  }

  for (const auto& s : scene_sound_names)
  {
    std::string s_path = s;
    if (s_path.find("*") != std::string::npos)
    {
      auto star = s_path.find("*");
      s_path.erase(star, 1);

      FMOD::Sound* sound = LoadSound(s_path.c_str(), true);
      if (sound != nullptr)
      {
        scene_sounds.Insert(s_path, sound);
      }
    }
    else
    {
      FMOD::Sound* sound = LoadSound(s_path.c_str(), false);
      if (sound != nullptr)
      {
        scene_sounds.Insert(s_path, sound);
      }
    }
  }

  while (std::getline(scene_stream, line))
  {
    if (line.find("#Entity") != std::string::npos)
    {
      Entity e;
      auto space = line.find(" ");
      auto new_line = line.substr(space + 1);

      auto comma = new_line.find(",");
      auto val_str = new_line.substr(0, comma);
      e.shader = scene_shaders.At(scene_shader_names[std::stoi(val_str)]);

      new_line = new_line.substr(comma + 2);
      comma = new_line.find(",");
      val_str = new_line.substr(0, comma);
      e.texture = scene_textures.At(scene_texture_names[std::stoi(val_str)]);

      new_line = new_line.substr(comma + 2);
      comma = new_line.find(",");
      val_str = new_line.substr(0, comma);
      f32 tmp_vec[2] = { 0.f, 0.f };

      space = val_str.find(" ");
      tmp_vec[0] = std::stof(val_str.substr(0, space));
      val_str = val_str.substr(space + 1);

      space = val_str.find(" ");
      tmp_vec[1] = std::stof(val_str.substr(0, space));

      e.position = vec2(tmp_vec[0], tmp_vec[1]);

      new_line = new_line.substr(comma + 2);
      comma = new_line.find(",");
      val_str = new_line.substr(space + 1);

      e.angle = std::stof(val_str);

      new_line = new_line.substr(comma + 2);
      comma = new_line.find(",");
      val_str = new_line.substr(0, comma);

      space = val_str.find(" ");
      tmp_vec[0] = std::stof(val_str.substr(0, space));
      val_str = val_str.substr(space + 1);

      space = val_str.find(" ");
      tmp_vec[1] = std::stof(val_str.substr(0, space));

      e.scale = vec2(tmp_vec[0], tmp_vec[1] * aspect_ratio);

      glGenVertexArrays(1, &e.vao);
      glGenBuffers(1, &e.vbo);
      glGenBuffers(1, &e.ebo);
      glBindVertexArray(e.vao);
      glBindBuffer(GL_ARRAY_BUFFER, e.vbo);
      glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), &e.verts[0], GL_STATIC_DRAW);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, e.ebo);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(u32), quad_indices, GL_STATIC_DRAW);
      glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(f32), (void*)0);
      glEnableVertexAttribArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindVertexArray(0);

      entities.PushBack(e);
      original_scales.PushBack(vec2(e.scale.x(), e.scale.y() / aspect_ratio));
    }
  }
}

en::vector<std::string> scene_names;

bool application_active = true;

void CloseWindow()
{
  application_active = false;
}

void Hit()
{
  PlaySound("Hit.wav");
}

enum class ANIM_STATE
{
  ACTIVE,
  INACTIVE
};

struct AnimInfo
{
  s32 anim_frame = 0;
  bool anim_flipped = false;
  TimerInfo anim_timer;
  ANIM_STATE anim_state = ANIM_STATE::INACTIVE;
};

AnimInfo megaman_anim = {};

const s32 MAX_PARTICLES = 5000;
s32 particle_textures[5];

struct
{
  u32 vao, vbo, ebo;
  en::vector<vec2> position;
  en::vector<vec2> scale;
  en::vector<vec4> color;
} particles;

void RunRight()
{
  megaman_anim.anim_flipped = false;
  megaman_anim.anim_state = ANIM_STATE::ACTIVE;
  u32 sprite_timer_value = GetTimerValue(megaman_anim.anim_timer);
  if (sprite_timer_value > 100)
  {
    if (megaman_anim.anim_frame == 9)
    {
      megaman_anim.anim_frame = 0;
    }
    else
    {
      megaman_anim.anim_frame++;
    }

    StartTimer(megaman_anim.anim_timer);
  }
}

void RunLeft()
{
  megaman_anim.anim_flipped = true;
  megaman_anim.anim_state = ANIM_STATE::ACTIVE;
  u32 sprite_timer_value = GetTimerValue(megaman_anim.anim_timer);
  if (sprite_timer_value > 100)
  {
    if (megaman_anim.anim_frame == 9)
    {
      megaman_anim.anim_frame = 0;
    }
    else
    {
      megaman_anim.anim_frame++;
    }

    StartTimer(megaman_anim.anim_timer);
  }
}

s32 main()
{
  aspect_ratio = (f32)window_width / (f32)window_height;
  const char* glsl_version = "#version 330";

  GLFWwindow* window;
  
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(window_width, window_height, "Hello Window", NULL, NULL);
  
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  glfwSetMouseButtonCallback(window, MouseButtonCallback);
  glfwSetKeyCallback(window, KeyCallback);
  glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

  glewInit();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
  ImGui::StyleColorsDark();

  ImGuiStyle& style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
  {
    style.WindowRounding = 0.f;
    style.Colors[ImGuiCol_WindowBg].w = 1.f;
  }

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

  if (!InitSound())
  {
    DebugPrintToConsole("Error: Could not init sound!");
  }

  StartTimer(game_timer);

  SetKeyboardInput(CloseWindow, GLFW_KEY_ESCAPE, BUTTON_ACTION::PRESS);
  SetKeyboardInput(Hit, GLFW_KEY_SPACE, BUTTON_ACTION::PRESS);

  SetKeyboardInput(RunRight, GLFW_KEY_D, BUTTON_ACTION::HOLD);
  SetKeyboardInput(RunLeft, GLFW_KEY_A, BUTTON_ACTION::HOLD);

  particle_textures[0] = LoadGLTexture("cloud0.png");
  particle_textures[1] = LoadGLTexture("cloud1.png");
  particle_textures[2] = LoadGLTexture("cloud2.png");
  particle_textures[3] = LoadGLTexture("cloud3.png");
  particle_textures[4] = LoadGLTexture("cloud4.png");

  Entity megaman;
  megaman.position = vec2(0.75f, 0.75f);
  megaman.scale = vec2(0.25f, 0.25f * aspect_ratio);
  megaman.shader = LoadGLShader("entity_animated.glsl");
  megaman.texture = LoadGLTexture("megaman_run.jpg");
  megaman.angle = 0.f;
  glGenVertexArrays(1, &megaman.vao);
  glGenBuffers(1, &megaman.vbo);
  glGenBuffers(1, &megaman.ebo);
  glBindVertexArray(megaman.vao);
  glBindBuffer(GL_ARRAY_BUFFER, megaman.vbo);
  glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), &megaman.verts[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, megaman.ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(u32), quad_indices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(f32), (void*)0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  entities.PushBack(megaman);
  original_scales.PushBack(vec2(megaman.scale.x(), megaman.scale.y() / aspect_ratio));
  StartTimer(megaman_anim.anim_timer);

  //Entity cloud;
  //cloud.position = vec2(0.85f, -0.85f);
  //cloud.scale = vec2(0.25f, 0.25f * aspect_ratio);
  //cloud.shader = LoadGLShader("entity_textured.glsl");
  //cloud.texture = TextureFromFile("cloud0.png");
  //cloud.angle = 0.f;
  //glGenVertexArrays(1, &cloud.vao);
  //glGenBuffers(1, &cloud.vbo);
  //glGenBuffers(1, &cloud.ebo);
  //glBindVertexArray(cloud.vao);
  //glBindBuffer(GL_ARRAY_BUFFER, cloud.vbo);
  //glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), &cloud.verts[0], GL_STATIC_DRAW);
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cloud.ebo);
  //glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(u32), quad_indices, GL_STATIC_DRAW);
  //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(f32), (void*)0);
  //glEnableVertexAttribArray(0);
  //glBindBuffer(GL_ARRAY_BUFFER, 0);
  //glBindVertexArray(0);
//
  //entities.PushBack(cloud);
  //original_scales.PushBack(vec2(cloud.scale.x(), cloud.scale.y() / aspect_ratio));

  en::vector<ParticleVertex> particle_verts;
  en::vector<u32> particle_indices;

  s32 particle_shader = LoadGLShader("particle_textured.glsl");
  glUseProgram(particle_shader);
  s32 samplers[5] = { 0, 1, 2, 3, 4 };
  glUniform1iv(glGetUniformLocation(particle_shader, "textures"), 5, samplers);

  for (s32 i = 0; i < MAX_PARTICLES; i++)
  {
    particles.position.PushBack(vec2(GetRandomFloatInRange(-1.f, 1.f), GetRandomFloatInRange(-1.f, 1.f)));
    particles.scale.PushBack(vec2(0.05f, 0.05f));
    particles.color.PushBack(vec4(GetRandomFloat(), GetRandomFloat(), GetRandomFloat(), GetRandomFloat()));

    particle_verts.PushBack({ vec2(0.1f + particles.position[i].x(), 0.1f + particles.position[i].y()), vec4(particles.color[i]), vec2(0.f, 0.f), 5.f * GetRandomFloat() });
    particle_verts.PushBack({ vec2(0.1f + particles.position[i].x(), -0.1f + particles.position[i].y()), vec4(particles.color[i]), vec2(1.f, 0.f), 5.f * GetRandomFloat() });
    particle_verts.PushBack({ vec2(-0.1f + particles.position[i].x(), -0.1f + particles.position[i].y()), vec4(particles.color[i]), vec2(1.f, 1.f), 5.f * GetRandomFloat() });
    particle_verts.PushBack({ vec2(-0.1f + particles.position[i].x(), 0.1f + particles.position[i].y()), vec4(particles.color[i]), vec2(0.f, 1.f), 5.f * GetRandomFloat() });

    particle_indices.PushBack(0 + i * 4);
    particle_indices.PushBack(1 + i * 4);
    particle_indices.PushBack(3 + i * 4);
    particle_indices.PushBack(1 + i * 4);
    particle_indices.PushBack(2 + i * 4);
    particle_indices.PushBack(3 + i * 4);
  }

  glGenVertexArrays(1, &particles.vao);
  glGenBuffers(1, &particles.vbo);
  glGenBuffers(1, &particles.ebo);
  glBindVertexArray(particles.vao);
  glBindBuffer(GL_ARRAY_BUFFER, particles.vbo);
  glBufferData(GL_ARRAY_BUFFER, particles.position.Size() * 4 * sizeof(ParticleVertex), &particle_verts[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, particles.ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, particles.position.Size() * 6 * sizeof(u32), &particle_indices[0], GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), (void*)(2 * sizeof(f32)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), (void*)(2 * sizeof(f32) + 4 * sizeof(f32)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), (void*)(2 * sizeof(f32) + 4 * sizeof(f32) + 2 * sizeof(f32)));
  glEnableVertexAttribArray(3);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  //LoadScene("test_scene.enscene");
  PlaySound("MainTheme.wav");

  bool show_demo_window = true;

  while (application_active)
  {
    glfwPollEvents();
    ProcessKeyboardInput(window);

    StartTimer(frame_timer);

    glClearColor(1.f, 0.8f, 0.7f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (entities.Size() > 0)
    {
      for (auto& e : entities)
      {
        mat4 transform = mat4::Identity();
        transform = mat4::Translate(vec3(e.position.x(), e.position.y(), 0.f));
        transform *= mat4::Rotate(e.angle, vec3(0.f, 0.f, 1.f));
        transform *= mat4::Scale(vec3(e.scale.x(), e.scale.y(), 1.f));

        glUseProgram(e.shader);
        glUniformMatrix4fv(glGetUniformLocation(e.shader, "transform"), 1, GL_FALSE, transform.elements);
        if (e.shader == megaman.shader)
        {
          if (megaman_anim.anim_state == ANIM_STATE::ACTIVE)
          {
            glUniform1i(glGetUniformLocation(e.shader, "anim_frame"), megaman_anim.anim_frame);
          }
          else
          {
            megaman_anim.anim_frame = 0;
            glUniform1i(glGetUniformLocation(e.shader, "anim_frame"), megaman_anim.anim_frame);
          }
        
          if (megaman_anim.anim_flipped)
          {
            glUniform1f(glGetUniformLocation(e.shader, "anim_flip_val"), -1.f);
          }
          else
          {
            glUniform1f(glGetUniformLocation(e.shader, "anim_flip_val"), 1.f);
          }
        }

        glBindVertexArray(e.vao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, e.texture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
      }
    }

    mat4 particle_transform = mat4::Identity();
    glUseProgram(particle_shader);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, particle_textures[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, particle_textures[1]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, particle_textures[2]);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, particle_textures[3]);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, particle_textures[4]);
    glUniformMatrix4fv(glGetUniformLocation(particle_shader, "transform"), 1, GL_FALSE, particle_transform.elements);

    glBindVertexArray(particles.vao);
    //glDrawElements(GL_TRIANGLES, 6 * particles.position.Size(), GL_UNSIGNED_INT, 0);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      glfwMakeContextCurrent(window);
    }

    glfwSwapBuffers(window);
    //DebugPrintToConsole("Frame Time: ", delta_time, "s");

    game_time = (f32)GetTimerValue(game_timer) / 1000.f;
    StopTimer(frame_timer);
    delta_time = frame_timer.time_delta / 1000.f;
    megaman_anim.anim_state = ANIM_STATE::INACTIVE;
  }

  DebugPrintToConsole("Clean program exit");

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
