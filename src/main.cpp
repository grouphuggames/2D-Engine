// FLAPPY BIRD CLONE TIME

#define GLEW_STATIC

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cmath>
#include <string>
#include <chrono>
#include <random>
#include <filesystem>
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "stb_image.h"


using s32 = int32_t;
using u32 = uint32_t;
using f32  = float;
using f64 = double;

const s32 window_width = 720;
const s32 window_height = 1280;

const f64 PI = 3.14159;

static inline f32 ToRadians(f32 degrees)
{
  return degrees * ((f32)PI / 180.f);
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

enum TIME
{
  NANOSECOND,
  MICROSECOND,
  MILLISECOND,
  SECOND
};

struct TimerInfo
{
  TIME time_scale;
  std::chrono::time_point<std::chrono::steady_clock> timer_start;
  std::chrono::time_point<std::chrono::steady_clock> timer_stop;
  u32 time_delta;
};

TimerInfo game_timer;
TimerInfo frame_timer;
f32 game_time = 0.f;

void StartTimer(TimerInfo& info)
{
  info.timer_start = std::chrono::high_resolution_clock::now();
}

u32 GetTimerValue(TimerInfo& info)
{
  auto current_time = std::chrono::high_resolution_clock::now();
  if (info.time_scale == NANOSECOND)
    return (u32)std::chrono::duration_cast<std::chrono::nanoseconds>(current_time - info.timer_start).count();
  else if (info.time_scale == MICROSECOND)
    return (u32)std::chrono::duration_cast<std::chrono::microseconds>(current_time - info.timer_start).count();
  else if (info.time_scale == MILLISECOND)
    return (u32)std::chrono::duration_cast<std::chrono::milliseconds>(current_time - info.timer_start).count();
    else if (info.time_scale == SECOND)
    return (u32)std::chrono::duration_cast<std::chrono::seconds>(current_time - info.timer_start).count();

  return 0;
}

void StopTimer(TimerInfo& info)
{
  info.timer_stop = std::chrono::high_resolution_clock::now();
  if (info.time_scale == NANOSECOND)
    info.time_delta = (u32)std::chrono::duration_cast<std::chrono::nanoseconds>(info.timer_stop - info.timer_start).count();
  else if (info.time_scale == MICROSECOND)
    info.time_delta = (u32)std::chrono::duration_cast<std::chrono::microseconds>(info.timer_stop - info.timer_start).count();
  else if (info.time_scale == MILLISECOND)
    info.time_delta = (u32)std::chrono::duration_cast<std::chrono::milliseconds>(info.timer_stop - info.timer_start).count();
    else if (info.time_scale == SECOND)
    info.time_delta = (u32)std::chrono::duration_cast<std::chrono::seconds>(info.timer_stop - info.timer_start).count();
}

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

  static f32 length(const vec2& vec)
	{
		return sqrtf(vec.x() * vec.x() + vec.y() * vec.y());
	}

	static vec2 normalize(const vec2& vec)
	{
		f32 k = length(vec);
		return vec2(vec.x() / k, vec.y() / k);
	}

  friend vec2 operator*(const f32 left, const vec2& right)
	{
		return vec2(right.x() * left, right.y() * left);
	}

  friend vec2 operator/(const vec2& left, const f32 right)
  {
    return vec2(left.x() / right, left.y() / right);
  }

  friend vec2 operator+(const vec2& left, const vec2& right)
	{
		return vec2(left.x() + right.x(), left.y() + right.y());
	}

	friend vec2 operator-(const vec2& left, const vec2& right)
	{
		return vec2(left.x() - right.x(), left.y() - right.y());
	}

	vec2& operator+=(const vec2& other)
	{
		*this = *this + other;
		return *this;
	}

	vec2& operator-=(const vec2& other)
	{
		*this = *this - other;
		return *this;
	}
};

void DebugPrintVec2(vec2 vec)
{
  std::cout << "{ " << vec.x() << ", " << vec.y() << " }\n";
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
  
  vec3(f32 x, f32 y, f32 z)
  {
    data[0] = x;
    data[1] = y;
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
  std::cout << "{ " << vec.x() << ", " << vec.y() << ", " << vec.z() << " }\n";
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
  std::cout << "{ " << vec.x() << ", " << vec.y() << ", " << vec.z() << ", " << vec.w() << " }\n";
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

struct AABB
{
  vec2 position;
  vec2 half_extents;
};

bool AABBAABBColliding(AABB a, AABB b)
{
  if (fabsf(a.position.x() - b.position.x()) > (a.half_extents.x() + b.half_extents.x()))  return false;
  if (fabsf(a.position.y() - b.position.y()) > (a.half_extents.y() + b.half_extents.y()))  return false;

  return true;
}

struct Vertex
{
public:
  vec2 position;
  vec4 color;
};

struct StaticEntity
{
public:
  Vertex verts[4] = { { vec2(-1.f, 1.f), vec4(0.f, 0.f, 0.f, 1.f) }, { vec2(-1.f, -1.f), vec4(0.f, 0.f, 0.f, 1.f) }, { vec2(1.f, 1.f), vec4(0.f, 0.f, 0.f, 1.f) }, { vec2(1.f, -1.f), vec4(0.f, 0.f, 0.f, 1.f) } };
  vec2 position;
  vec2 scale;
  u32 vao, vbo;
  u32 shader;
  u32 texture;
  f32 angle = 0.f;
};

struct DynamicEntity
{
public:
  Vertex verts[4] = { { vec2(-1.f, 1.f), vec4(0.f, 0.f, 0.f, 1.f) }, { vec2(-1.f, -1.f), vec4(0.f, 0.f, 0.f, 1.f) }, { vec2(1.f, 1.f), vec4(0.f, 0.f, 0.f, 1.f) }, { vec2(1.f, -1.f), vec4(0.f, 0.f, 0.f, 1.f) } };
  vec2 position;
  vec2 scale;
  u32 vao, vbo;
  u32 shader;
  u32 texture;
  f32 angle = 0.f;
  AABB collision;
};

struct ScoreGate
{
public:
  AABB collision;
};

f32 delta_time = 0.f;
f32 last_frame = 0.f;

std::string AssetPath(const char* path)
{
  std::string prefix = "../../";
  return prefix.append(path);
}

u32 TextureFromFile(const char* path, bool gamma = false)
{
  std::string filename = AssetPath(path);

  u32 texture_id;
  glGenTextures(1, &texture_id);

  s32 width, height, nr_components;
  unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nr_components, 0);
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
    std::cout << "Successfully loaded texture: " << filename << '\n';
  }
  else
  {
    std::cout << "Texture failed to load: " << path << '\n';
    stbi_image_free(data);
  }

  return texture_id;
}

enum SHADER_TYPE
{
  NONE = -1,
  VERTEX,
  FRAGMENT
};

u32 CreateGLShader(const char* filepath)
{
  std::ifstream stream(AssetPath(filepath));
  std::string line;
  std::stringstream shader_streams[2];
  SHADER_TYPE type = NONE;

  while (std::getline(stream, line))
  {
    if (line.find("#shader") != std::string::npos)
    {
      if (line.find("vertex") != std::string::npos)
      {
	type = VERTEX;
      }
      else if (line.find("fragment") != std::string::npos)
      {
	type = FRAGMENT;
      }
      else
      {
	char error_type[128] = "Unsupported shader type: ";
	strcat(error_type, line.c_str());
	std::cout << "Failed to load shader: " << filepath << '\n';
	std::cout << error_type << '\n';
      }
    }
    else
    {
      shader_streams[(s32)type] << line << '\n';
    }
  }

  stream.close();

  u32 vs = glCreateShader(GL_VERTEX_SHADER);
  std::string vs_source_data = shader_streams[VERTEX].str();
  const char* vs_source = vs_source_data.c_str();
  glShaderSource(vs, 1, &vs_source, nullptr);
  glCompileShader(vs);
  s32 success = 0;
  char log_info[512];
  glGetShaderiv(vs, GL_COMPILE_STATUS, &success);

  if (!success)
  {
    glGetShaderInfoLog(vs, 512, nullptr, log_info);
    std::cout << "Failed to load shader: " << filepath << '\n';
    std::cout << log_info << "\n\n\n";

    return 9999;
  }

  u32 fs = glCreateShader(GL_FRAGMENT_SHADER);
  std::string fs_source_data = shader_streams[FRAGMENT].str();
  const char* fs_source = fs_source_data.c_str();
  glShaderSource(fs, 1, &fs_source, nullptr);
  glCompileShader(fs);
  glGetShaderiv(fs, GL_COMPILE_STATUS, &success);

  if (!success)
  {
    glGetShaderInfoLog(fs, 512, nullptr, log_info);
    std::cout << "Failed to load shader: " << filepath << '\n';
    std::cout << log_info << "\n\n\n";

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
    std::cout << "Failed to load shader: " << filepath << '\n';
    std::cout << log_info << "\n\n\n";

    return 9999;
  }

  glUseProgram(program);
  glDeleteShader(vs);
  glDeleteShader(fs);
  shader_streams[0].str("");
  shader_streams[1].str("");

  std::cout << "Successfully loaded shader: " << filepath << '\n';
  
  return program;
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

en::vector<StaticEntity> static_entities;
en::vector<DynamicEntity> dynamic_entities;
en::vector<ScoreGate> score_gates;
en::vector<u32> scene_shaders;
en::vector<u32> scene_textures;

en::vector<std::string> LoadSceneSelector()
{
  en::vector<std::string> result;
  
  for (const auto& file : std::filesystem::directory_iterator(AssetPath("")))
  {
    std::string file_path = file.path().string();
    
    if (file_path.find(".enscene") != std::string::npos)
    {
      result.PushBack(file_path);
    }
  }

  return result;
}

void LoadScene(const char* scene_path)
{
  static_entities.Clear();
  dynamic_entities.Clear();
  scene_shaders.Clear();
  scene_textures.Clear();

  std::ifstream scene_stream(scene_path);

  std::string line;

  en::vector<std::string> scene_shader_names;
  en::vector<std::string> scene_texture_names;

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
    else
    {
      break;
    }
  }

  for (const auto& s : scene_shader_names)
  {
    std::string s_path = s;
    scene_shaders.PushBack(CreateGLShader(s_path.c_str()));
  }

  for (const auto& t : scene_texture_names)
  {
    std::string t_path = t;
    scene_textures.PushBack(TextureFromFile(t_path.c_str()));
  }

  while (std::getline(scene_stream, line))
  {
    if (line.find("#StaticEntity") != std::string::npos)
    {
      StaticEntity e;
      auto space = line.find(" ");
      auto new_line = line.substr(space + 1);

      auto comma = new_line.find(",");
      auto val_str = new_line.substr(0, comma);
      e.shader = scene_shaders[std::stoi(val_str)];

      new_line = new_line.substr(comma + 2);
      comma = new_line.find(",");
      val_str = new_line.substr(0, comma);
      e.texture = scene_textures[std::stoi(val_str)];

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

      e.scale = vec2(tmp_vec[0], tmp_vec[1]);

      new_line = new_line.substr(comma + 2);
      comma = new_line.find(",");
      val_str = new_line.substr(0, comma);
      f32 tmp_vec4[4] = { 0.f, 0.f, 0.f, 0.f };

      space = val_str.find(" ");
      tmp_vec4[0] = std::stof(val_str.substr(0, space));
      val_str = val_str.substr(space + 1);

      space = val_str.find(" ");
      tmp_vec4[1] = std::stof(val_str.substr(0, space));
      val_str = val_str.substr(space + 1);

      space = val_str.find(" ");
      tmp_vec4[2] = std::stof(val_str.substr(0, space));
      val_str = val_str.substr(space + 1);

      space = val_str.find(" ");
      tmp_vec4[3] = std::stof(val_str.substr(0, space));
      val_str = val_str.substr(space + 1);

      e.verts[0].color = vec4(tmp_vec4[0], tmp_vec4[1], tmp_vec4[2], tmp_vec4[3]);

      new_line = new_line.substr(comma + 2);
      comma = new_line.find(",");
      val_str = new_line.substr(0, comma);
	
      space = val_str.find(" ");
      tmp_vec4[0] = std::stof(val_str.substr(0, space));
      val_str = val_str.substr(space + 1);

      space = val_str.find(" ");
      tmp_vec4[1] = std::stof(val_str.substr(0, space));
      val_str = val_str.substr(space + 1);

      space = val_str.find(" ");
      tmp_vec4[2] = std::stof(val_str.substr(0, space));
      val_str = val_str.substr(space + 1);

      space = val_str.find(" ");
      tmp_vec4[3] = std::stof(val_str.substr(0, space));
      val_str = val_str.substr(space + 1);

      e.verts[1].color = vec4(tmp_vec4[0], tmp_vec4[1], tmp_vec4[2], tmp_vec4[3]);
	
      new_line = new_line.substr(comma + 2);
      comma = new_line.find(",");
      val_str = new_line.substr(0, comma);
	
      space = val_str.find(" ");
      tmp_vec4[0] = std::stof(val_str.substr(0, space));
      val_str = val_str.substr(space + 1);

      space = val_str.find(" ");
      tmp_vec4[1] = std::stof(val_str.substr(0, space));
      val_str = val_str.substr(space + 1);

      space = val_str.find(" ");
      tmp_vec4[2] = std::stof(val_str.substr(0, space));
      val_str = val_str.substr(space + 1);

      space = val_str.find(" ");
      tmp_vec4[3] = std::stof(val_str.substr(0, space));
      val_str = val_str.substr(space + 1);

      e.verts[2].color = vec4(tmp_vec4[0], tmp_vec4[1], tmp_vec4[2], tmp_vec4[3]);

      new_line = new_line.substr(comma + 2);
      comma = new_line.find(",");
      val_str = new_line.substr(0, comma);
	
      space = val_str.find(" ");
      tmp_vec4[0] = std::stof(val_str.substr(0, space));
      val_str = val_str.substr(space + 1);

      space = val_str.find(" ");
      tmp_vec4[1] = std::stof(val_str.substr(0, space));
      val_str = val_str.substr(space + 1);

      space = val_str.find(" ");
      tmp_vec4[2] = std::stof(val_str.substr(0, space));
      val_str = val_str.substr(space + 1);

      space = val_str.find(" ");
      tmp_vec4[3] = std::stof(val_str.substr(0, space));
      val_str = val_str.substr(space + 1);

      e.verts[3].color = vec4(tmp_vec4[0], tmp_vec4[1], tmp_vec4[2], tmp_vec4[3]);

      glGenVertexArrays(1, &e.vao);
      glBindVertexArray(e.vao);
      glGenBuffers(1, &e.vbo);
      glBindBuffer(GL_ARRAY_BUFFER, e.vbo);
      glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), &e.verts[0], GL_STATIC_DRAW);

      static_entities.PushBack(e);
    }
    else if (line.find("#DynamicEntity") != std::string::npos)
    {
      DynamicEntity e;
      auto space = line.find(" ");
      auto new_line = line.substr(space + 1);

      auto comma = new_line.find(",");
      auto val_str = new_line.substr(0, comma);
      e.shader = scene_shaders[std::stoi(val_str)];

      new_line = new_line.substr(comma + 2);
      comma = new_line.find(",");
      val_str = new_line.substr(0, comma);
      e.texture = scene_textures[std::stoi(val_str)];

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

      e.scale = vec2(tmp_vec[0], tmp_vec[1]);

      new_line = new_line.substr(comma + 2);
      comma = new_line.find(",");
      val_str = new_line.substr(0, comma);
      f32 tmp_vec4[4] = { 0.f, 0.f, 0.f, 0.f };

      space = val_str.find(" ");
      tmp_vec4[0] = std::stof(val_str.substr(0, space));
      val_str = val_str.substr(space + 1);

      space = val_str.find(" ");
      tmp_vec4[1] = std::stof(val_str.substr(0, space));
      val_str = val_str.substr(space + 1);

      space = val_str.find(" ");
      tmp_vec4[2] = std::stof(val_str.substr(0, space));
      val_str = val_str.substr(space + 1);

      space = val_str.find(" ");
      tmp_vec4[3] = std::stof(val_str.substr(0, space));
      val_str = val_str.substr(space + 1);

      e.verts[0].color = vec4(tmp_vec4[0], tmp_vec4[1], tmp_vec4[2], tmp_vec4[3]);

      new_line = new_line.substr(comma + 2);
      comma = new_line.find(",");
      val_str = new_line.substr(0, comma);
	
      space = val_str.find(" ");
      tmp_vec4[0] = std::stof(val_str.substr(0, space));
      val_str = val_str.substr(space + 1);

      space = val_str.find(" ");
      tmp_vec4[1] = std::stof(val_str.substr(0, space));
      val_str = val_str.substr(space + 1);

      space = val_str.find(" ");
      tmp_vec4[2] = std::stof(val_str.substr(0, space));
      val_str = val_str.substr(space + 1);

      space = val_str.find(" ");
      tmp_vec4[3] = std::stof(val_str.substr(0, space));
      val_str = val_str.substr(space + 1);

      e.verts[1].color = vec4(tmp_vec4[0], tmp_vec4[1], tmp_vec4[2], tmp_vec4[3]);
	
      new_line = new_line.substr(comma + 2);
      comma = new_line.find(",");
      val_str = new_line.substr(0, comma);
	
      space = val_str.find(" ");
      tmp_vec4[0] = std::stof(val_str.substr(0, space));
      val_str = val_str.substr(space + 1);

      space = val_str.find(" ");
      tmp_vec4[1] = std::stof(val_str.substr(0, space));
      val_str = val_str.substr(space + 1);

      space = val_str.find(" ");
      tmp_vec4[2] = std::stof(val_str.substr(0, space));
      val_str = val_str.substr(space + 1);

      space = val_str.find(" ");
      tmp_vec4[3] = std::stof(val_str.substr(0, space));
      val_str = val_str.substr(space + 1);

      e.verts[2].color = vec4(tmp_vec4[0], tmp_vec4[1], tmp_vec4[2], tmp_vec4[3]);

      new_line = new_line.substr(comma + 2);
      comma = new_line.find(",");
      val_str = new_line.substr(0, comma);
	
      space = val_str.find(" ");
      tmp_vec4[0] = std::stof(val_str.substr(0, space));
      val_str = val_str.substr(space + 1);

      space = val_str.find(" ");
      tmp_vec4[1] = std::stof(val_str.substr(0, space));
      val_str = val_str.substr(space + 1);

      space = val_str.find(" ");
      tmp_vec4[2] = std::stof(val_str.substr(0, space));
      val_str = val_str.substr(space + 1);

      space = val_str.find(" ");
      tmp_vec4[3] = std::stof(val_str.substr(0, space));
      val_str = val_str.substr(space + 1);

      e.verts[3].color = vec4(tmp_vec4[0], tmp_vec4[1], tmp_vec4[2], tmp_vec4[3]);

      glGenVertexArrays(1, &e.vao);
      glBindVertexArray(e.vao);
      glGenBuffers(1, &e.vbo);
      glBindBuffer(GL_ARRAY_BUFFER, e.vbo);
      glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), &e.verts[0], GL_STATIC_DRAW);

      e.collision.position = e.position;
      e.collision.half_extents = e.scale;

      dynamic_entities.PushBack(e);
    }
  }
}

en::vector<std::string> scene_names;

vec4 background_color = vec4(135.f / 255.f, 196.f / 255.f, 255.f / 255.f, 1.f);
bool gameplay_active = false;
f32 bird_velocity = 0.f;
const f32 gravity = -0.0000002f;

void ProcessKeyboardInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  glfwSetWindowShouldClose(window, true);
}

void GameKeyCallback(GLFWwindow* window, s32 key, s32 scancode, s32 action, s32 mods)
{
  if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
  {
    bird_velocity = 0.119f;
  }
}

void MainMenuKeyCallback(GLFWwindow* window, s32 key, s32 scancode, s32 action, s32 mods)
{
  if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
  {
    LoadScene(AssetPath("game_level.enscene").c_str());
    gameplay_active = true;
    dynamic_entities[0].collision.half_extents = vec2(0.1f, 0.121f);
    glfwSetKeyCallback(window, GameKeyCallback);
  }
}

s32 main()
{ 
  GLFWwindow* window;
  
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(window_width, window_height, "Flappy Bird", NULL, NULL);
  
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  glfwSetMouseButtonCallback(window, MouseButtonCallback);
  glfwSetKeyCallback(window, MainMenuKeyCallback);

  glewInit();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

  StartTimer(game_timer);

  TimerInfo pillar_timer;
  bool spawn_pillar = false;
  pillar_timer.time_scale = MILLISECOND;
  StartTimer(pillar_timer);
  const f32 pillar_distance = 1.753f;

  TimerInfo cloud_timer;
  bool spawn_clouds = false;
  cloud_timer.time_scale = MILLISECOND;
  StartTimer(cloud_timer);

  LoadScene(AssetPath("title.enscene").c_str());

  s32 score = 0;
  TimerInfo score_timer;
  score_timer.time_scale = MILLISECOND;
  StartTimer(score_timer);
  bool currently_scoring = false;

  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();
    StartTimer(frame_timer);

    glClearColor(background_color.x(), background_color.y(), background_color.z(), background_color.w());
    glClear(GL_COLOR_BUFFER_BIT);

    ProcessKeyboardInput(window);

    if (gameplay_active)
    {
      bird_velocity = bird_velocity + gravity * delta_time;
      if (bird_velocity > -0.225) dynamic_entities[0].angle = 2.41 * bird_velocity;
      dynamic_entities[0].position.data[1] += delta_time * bird_velocity * 0.00001f;
      dynamic_entities[0].collision.position.data[1] = dynamic_entities[0].position.data[1];

      for (auto& s : static_entities)
      {
        s.position.data[0] -= delta_time * 0.0000007f * s.scale.x();
      }

      for (s32 i = 1; i < dynamic_entities.Size(); i++)
      {
        dynamic_entities[i].position.data[0] -= delta_time * 0.0000007f;
        dynamic_entities[i].collision.position.data[0] = dynamic_entities[i].position.data[0] - 0.039f;
        dynamic_entities[i].collision.position.data[1] = dynamic_entities[i].position.data[1] + 0.204f;
      }

      for (auto& g : score_gates)
      {
        g.collision.position.data[0] -= delta_time * 0.0000007f;
      }

      for (s32 i = 1; i < dynamic_entities.Size(); i++)
      {
        if (AABBAABBColliding(dynamic_entities[0].collision, dynamic_entities[i].collision))
        {
          gameplay_active = false;
        }
      }

      for (auto& g : score_gates)
      {
        if (AABBAABBColliding(dynamic_entities[0].collision, g.collision) && (u32)GetTimerValue(score_timer) > 750)
        {
          score++;
          currently_scoring = true;
          StartTimer(score_timer);
          std::cout << score << '\n';
        }
      }

      if (GetTimerValue(pillar_timer) > 2800)
      {
        spawn_pillar = true;
      }

      if (spawn_pillar)
      {
        DynamicEntity pillar0;
        pillar0.position = vec2(1.5f, GetRandomFloatInRange(0.215f, 1.118f));
        pillar0.scale = vec2(1.f, 1.f);
        pillar0.shader = scene_shaders[0];
        pillar0.texture = scene_textures[8];

        glGenVertexArrays(1, &pillar0.vao);
        glBindVertexArray(pillar0.vao);
        glGenBuffers(1, &pillar0.vbo);
        glBindBuffer(GL_ARRAY_BUFFER, pillar0.vbo);
        glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), &pillar0.verts[0], GL_STATIC_DRAW);

        pillar0.collision.position = pillar0.position;
        pillar0.collision.half_extents = vec2(0.157, 0.552f);

        if (dynamic_entities.Size() > 13)
        {
          f32 furthest_pillar_distance = 9999.f;
          s32 furthest_pillar_index = 9999;
          for (s32 i = 1; i < dynamic_entities.Size(); i++)
          {
            if (furthest_pillar_distance > dynamic_entities[i].position.x())
            {
              furthest_pillar_distance = dynamic_entities[i].position.x();
              furthest_pillar_index = i;
            }
          }

          dynamic_entities[furthest_pillar_index] = pillar0;

          if (score_gates.Size() > 6)
          {
            f32 furthest_collision_distance = 9999.f;
            s32 furthest_collision_index = 9999;
            for (s32 i = 0; i < score_gates.Size(); i++)
            {
              if (furthest_collision_distance > score_gates[i].collision.position.x())
              {
                furthest_collision_distance = score_gates[i].collision.position.x();
                furthest_collision_index = i;
              }
            }

            ScoreGate score_gate;
            score_gate.collision.position = vec2(pillar0.position.x(), pillar0.position.y() - 0.65f);
            score_gate.collision.half_extents = vec2(0.025f, 0.248f);
            score_gates[furthest_collision_index] = score_gate;
          }
        }
        else
        {
          dynamic_entities.PushBack(pillar0);

          ScoreGate score_gate;
          score_gate.collision.position = vec2(pillar0.position.x(), pillar0.position.y() - 0.65f);
          score_gate.collision.half_extents = vec2(0.025f, 0.248f);
          score_gates.PushBack(score_gate);
        }

        DynamicEntity pillar1;
        pillar1.position = vec2(1.5f, pillar0.position.data[1] - pillar_distance);
        pillar1.scale = vec2(1.f, 1.f);
        pillar1.shader = scene_shaders[0];
        pillar1.texture = scene_textures[8];

        glGenVertexArrays(1, &pillar1.vao);
        glBindVertexArray(pillar1.vao);
        glGenBuffers(1, &pillar1.vbo);
        glBindBuffer(GL_ARRAY_BUFFER, pillar1.vbo);
        glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), &pillar1.verts[0], GL_STATIC_DRAW);

        pillar1.collision.position = pillar1.position;
        pillar1.collision.half_extents = vec2(0.157, 0.552f);

        if (dynamic_entities.Size() > 13)
        {
          f32 furthest_pillar_distance = 9999.f;
          s32 furthest_pillar_index = 9999;
          for (s32 i = 1; i < dynamic_entities.Size(); i++)
          {
            if (furthest_pillar_distance > dynamic_entities[i].position.x())
            {
              furthest_pillar_distance = dynamic_entities[i].position.x();
              furthest_pillar_index = i;
            }
          }

          dynamic_entities[furthest_pillar_index] = pillar1;
        }
        else
        {
          dynamic_entities.PushBack(pillar1);
        }

        spawn_pillar = false;
        StartTimer(pillar_timer);
      }

      if (GetTimerValue(cloud_timer) > 1800)
      {
        spawn_clouds = true;
      }

      if (spawn_clouds)
      {
        StaticEntity cloud0;
        cloud0.position = vec2(1.5f, GetRandomFloatInRange(-1.f, -0.5f));
        cloud0.angle = GetRandomFloatInRange(0.f, ToRadians(180.f));
        f32 scale = GetRandomFloatInRange(0.1f, 0.34f);
        cloud0.scale = vec2(scale, scale);
        cloud0.shader = scene_shaders[0];
        cloud0.texture = scene_textures[std::lround(GetRandomFloatInRange(1.f, 7.f))];

        glGenVertexArrays(1, &cloud0.vao);
        glBindVertexArray(cloud0.vao);
        glGenBuffers(1, &cloud0.vbo);
        glBindBuffer(GL_ARRAY_BUFFER, cloud0.vbo);
        glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), &cloud0.verts[0], GL_STATIC_DRAW);

        if (static_entities.Size() > 27)
        {
          f32 furthest_cloud_distance = 9999.f;
          s32 furthest_cloud_index = 9999;
          for (s32 i = 0; i < static_entities.Size(); i++)
          {
            if (furthest_cloud_distance > static_entities[i].position.x())
            {
              furthest_cloud_distance = static_entities[i].position.x();
              furthest_cloud_index = i;
            }
          }

          static_entities[furthest_cloud_index] = cloud0;
        }
        else
        {
          static_entities.PushBack(cloud0);
        }

        StaticEntity cloud1;
        cloud1.position = vec2(1.5f + GetRandomFloatInRange(0.f, 0.4f), GetRandomFloatInRange(-0.5f, 0.5f));
        cloud1.angle = GetRandomFloatInRange(0.f, ToRadians(180.f));
        cloud1.scale = vec2(scale, scale);
        cloud1.shader = scene_shaders[0];
        cloud1.texture = scene_textures[std::lround(GetRandomFloatInRange(1.f, 7.f))];

        glGenVertexArrays(1, &cloud1.vao);
        glBindVertexArray(cloud1.vao);
        glGenBuffers(1, &cloud1.vbo);
        glBindBuffer(GL_ARRAY_BUFFER, cloud1.vbo);
        glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), &cloud1.verts[0], GL_STATIC_DRAW);

        if (static_entities.Size() > 40)
        {
          f32 furthest_cloud_distance = 9999.f;
          s32 furthest_cloud_index = 9999;
          for (s32 i = 0; i < static_entities.Size(); i++)
          {
            if (furthest_cloud_distance > static_entities[i].position.x())
            {
              furthest_cloud_distance = static_entities[i].position.x();
              furthest_cloud_index = i;
            }
          }

          static_entities[furthest_cloud_index] = cloud1;
        }
        else
        {
          static_entities.PushBack(cloud1);
        }

        StaticEntity cloud2;
        cloud2.position = vec2(1.5f + GetRandomFloatInRange(0.f, 0.4f), GetRandomFloatInRange(0.5f, 1.f));
        cloud2.angle = GetRandomFloatInRange(0.f, ToRadians(180.f));
        cloud2.scale = vec2(scale, scale);
        cloud2.shader = scene_shaders[0];
        cloud2.texture = scene_textures[std::lround(GetRandomFloatInRange(1.f, 7.f))];

        glGenVertexArrays(1, &cloud2.vao);
        glBindVertexArray(cloud2.vao);
        glGenBuffers(1, &cloud2.vbo);
        glBindBuffer(GL_ARRAY_BUFFER, cloud2.vbo);
        glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), &cloud2.verts[0], GL_STATIC_DRAW);

        if (static_entities.Size() > 40)
        {
          f32 furthest_cloud_distance = 9999.f;
          s32 furthest_cloud_index = 9999;
          for (s32 i = 0; i < static_entities.Size(); i++)
          {
            if (furthest_cloud_distance > static_entities[i].position.x())
            {
              furthest_cloud_distance = static_entities[i].position.x();
              furthest_cloud_index = i;
            }
          }

          static_entities[furthest_cloud_index] = cloud2;
        }
        else
        {
          static_entities.PushBack(cloud2);
        }

        spawn_clouds = false;
        StartTimer(cloud_timer);
      }
    }

    for (auto& e : static_entities)
    {
      mat4 transform = mat4::Identity();
      transform = mat4::Translate(vec3(e.position.x(), e.position.y(), 0.f));
      transform *= mat4::Rotate(e.angle, vec3(0.f, 0.f, 1.f));
      transform *= mat4::Scale(vec3(e.scale.x(), e.scale.y(), 1.f));

      glUseProgram(e.shader);
      glUniformMatrix4fv(glGetUniformLocation(e.shader, "transform"), 1, GL_FALSE, transform.elements);
      glBindVertexArray(e.vao);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (void*)0);
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (void*)(sizeof(vec2)));
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, e.texture);
      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    for (auto& e : dynamic_entities)
    {
      mat4 transform = mat4::Identity();
      transform = mat4::Translate(vec3(e.position.x(), e.position.y(), 0.f));
      transform *= mat4::Rotate(e.angle, vec3(0.f, 0.f, 1.f));
      transform *= mat4::Scale(vec3(e.scale.x(), e.scale.y(), 1.f));

      glUseProgram(e.shader);
      glUniformMatrix4fv(glGetUniformLocation(e.shader, "transform"), 1, GL_FALSE, transform.elements);
      glBindVertexArray(e.vao);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (void*)0);
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (void*)(sizeof(vec2)));
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, e.texture);
      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // draw ui here
    ImGui::Begin("Menu");
    
    if (ImGui::Button("Load Scene"))
    {
      scene_names = LoadSceneSelector();
    }

    ImGui::ColorEdit3("Color", &background_color.data[0]);

    for (const auto& scene_name : scene_names)
    {
      if (ImGui::Button(scene_name.c_str()))
      {
        LoadScene(scene_name.c_str());
        scene_names.Clear();
      }
    }
    
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);

    game_time = (f32)GetTimerValue(game_timer) / 1000.f;
    StopTimer(frame_timer);
    delta_time = frame_timer.time_delta / 1000.f;
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
