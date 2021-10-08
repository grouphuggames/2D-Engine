// this will be the source code for the pong game inside of the engine

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
#include "fmod.hpp"
#include "stb_image.h"


using s32 = int32_t;
using u32 = uint32_t;
using f32  = float;
using f64 = double;

const s32 window_width = 1280;
const s32 window_height = 720;

const f32 aspect_ratio = (f32)window_width / (f32)window_height;

const f64 PI = 3.14159;

#define PADDLE_LEFT 0
#define PADDLE_RIGHT 1
#define BOUNDARY_TOP 2
#define BOUNDARY_BOTTOM 3
#define BOUNDARY_LEFT 4
#define BOUNDARY_RIGHT 5
#define SCORE_PLAYER_1 6
#define SCORE_PLAYER_2 7
#define PONG 8

FMOD::System* audio_system;

static inline f32 ToRadians(f32 degrees)
{
  return degrees * ((f32)PI / 180.f);
}

enum SUPER_TIME
{
  NANOSECOND,
  MICROSECOND,
  MILLISECOND
};

struct TimerInfo
{
  SUPER_TIME time_scale;
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
    return (u32)std::chrono::duration_cast<std::chrono::nanoseconds>(info.timer_stop - info.timer_start).count();
  else if (info.time_scale == MICROSECOND)
    return (u32)std::chrono::duration_cast<std::chrono::microseconds>(info.timer_stop - info.timer_start).count();
  else if (info.time_scale == MILLISECOND)
    return (u32)std::chrono::duration_cast<std::chrono::milliseconds>(info.timer_stop - info.timer_start).count();

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
};

struct Vertex
{
public:
  vec2 position;
};

struct Entity
{
public:
  Vertex verts[4] = { vec2(-1.f, 1.f), vec2(-1.f, -1.f), vec2(1.f, 1.f), vec2(1.f, -1.f) };
  vec2 position;
  vec2 scale;
  vec4 color;
  u32 vao, vbo;
  u32 shader;
  s32 texture = -1;
  f32 angle = 0.f;
  AABB collision;
  bool visible = true;
};

f32 delta_time = 0.f;
f32 last_frame = 0.f;

std::string AssetPath(const char* path)
{
  std::string prefix = "../../";
  return prefix.append(path);
}

std::string AssetPath(std::string path)
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

en::vector<Entity> entities;
en::vector<u32> scene_shaders;
en::vector<u32> scene_textures;
en::vector<FMOD::Sound*> scene_sounds;

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
  entities.Clear();
  scene_shaders.Clear();
  scene_textures.Clear();
  scene_sounds.Clear();

  std::ifstream scene_stream(scene_path);

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
    scene_shaders.PushBack(CreateGLShader(s_path.c_str()));
  }

  for (const auto& t : scene_texture_names)
  {
    std::string t_path = t;
    scene_textures.PushBack(TextureFromFile(t_path.c_str()));
  }

  for (const auto& s : scene_sound_names)
  {
    std::string s_path = s;
    if (s_path.find("*") != std::string::npos)
    {
      auto star = s_path.find("*");
      s_path.erase(star, 1);
      std::cout << s_path << '\n';

      FMOD::Sound* sound;
      if (audio_system->createSound(AssetPath(s_path).c_str(), FMOD_DEFAULT | FMOD_LOOP_NORMAL, 0, &sound) == FMOD_OK)
      {
        std::cout << "Successfully loaded audio asset: " << s_path << '\n';
        scene_sounds.PushBack(sound);
      }
      else
      {
        std::cout << "Failed to load audio asset: " << s_path << '\n';
      }
    }
    else
    {
      FMOD::Sound* sound;
      if (audio_system->createSound(AssetPath(s_path).c_str(), FMOD_DEFAULT, 0, &sound) == FMOD_OK)
      {
        std::cout << "Successfully loaded audio asset: " << s_path << '\n';
        scene_sounds.PushBack(sound);
      }
      else
      {
        std::cout << "Failed to load audio asset: " << s_path << '\n';
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
      e.shader = scene_shaders[std::stoi(val_str)];

      new_line = new_line.substr(comma + 2);
      comma = new_line.find(",");
      val_str = new_line.substr(0, comma);
      if (std::stoi(val_str) >= 0)
      {
        e.texture = scene_textures[std::stoi(val_str)];
      }

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

      e.color = vec4(tmp_vec4[0], tmp_vec4[1], tmp_vec4[2], tmp_vec4[3]);

      e.collision.position = e.position;
      e.collision.half_extents = e.scale;

      glGenVertexArrays(1, &e.vao);
      glBindVertexArray(e.vao);
      glGenBuffers(1, &e.vbo);
      glBindBuffer(GL_ARRAY_BUFFER, e.vbo);
      glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), &e.verts[0], GL_STATIC_DRAW);

      entities.PushBack(e);
    }
  }
}

void ProcessKeyboardInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  glfwSetWindowShouldClose(window, true);

  f32 movement_speed = 0.000001f * delta_time;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {
    entities[PADDLE_LEFT].position += vec2(0.f, movement_speed);
    entities[PADDLE_LEFT].collision.position = entities[PADDLE_LEFT].position;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
    entities[PADDLE_LEFT].position -= vec2(0.f, movement_speed);
    entities[PADDLE_LEFT].collision.position = entities[PADDLE_LEFT].position;
  }

  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
  {
    entities[PADDLE_RIGHT].position += vec2(0.f, movement_speed);
    entities[PADDLE_RIGHT].collision.position = entities[PADDLE_RIGHT].position;
  }
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
  {
    entities[PADDLE_RIGHT].position -= vec2(0.f, movement_speed);
    entities[PADDLE_RIGHT].collision.position = entities[PADDLE_RIGHT].position;
  }
}

s32 main()
{ 
  GLFWwindow* window;
  
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(window_width, window_height, "Hello Window", NULL, NULL);
  
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  glewInit();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  StartTimer(game_timer);

  FMOD::Channel* audio_channel0;
  FMOD::Channel* audio_channel1;
  FMOD::Channel* audio_channel2;
  FMOD::System_Create(&audio_system);

  audio_system->init(64, FMOD_INIT_NORMAL, 0);

  LoadScene(AssetPath("pong.enscene").c_str());

  vec2 pong_direction = vec2(GetRandomFloatInRange(-1.f, 1.f), GetRandomFloatInRange(-1.f, 1.f));
  pong_direction = vec2::normalize(pong_direction);

  s32 hits = 0;
  f32 speed;

  s32 player_1_score = 0;
  s32 player_2_score = 0;

  bool gameplay_active = true;

  audio_system->playSound(scene_sounds[0], nullptr, false, &audio_channel0);

  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();
    StartTimer(frame_timer);

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (gameplay_active)
    {
      ProcessKeyboardInput(window);

      if (AABBAABBColliding(entities[PADDLE_LEFT].collision, entities[PONG].collision))
      {
        pong_direction = vec2(-1.f * pong_direction.x(), pong_direction.y());
        audio_system->playSound(scene_sounds[1], nullptr, false, &audio_channel1);
        hits++;
      }

      if (AABBAABBColliding(entities[PADDLE_RIGHT].collision, entities[PONG].collision))
      {
        pong_direction = vec2(-1.f * pong_direction.x(), pong_direction.y());
        audio_system->playSound(scene_sounds[1], nullptr, false, &audio_channel1);
        hits++;
      }

      if (AABBAABBColliding(entities[BOUNDARY_BOTTOM].collision, entities[PONG].collision))
      {
        pong_direction = vec2(pong_direction.x(), -1.f * pong_direction.y());
        audio_system->playSound(scene_sounds[1], nullptr, false, &audio_channel1);
        hits++;
      }

      if (AABBAABBColliding(entities[BOUNDARY_TOP].collision, entities[PONG].collision))
      {
        pong_direction = vec2(pong_direction.x(), -1.f * pong_direction.y());
        audio_system->playSound(scene_sounds[1], nullptr, false, &audio_channel1);
        hits++;
      }

      if (AABBAABBColliding(entities[BOUNDARY_LEFT].collision, entities[PONG].collision))
      {
        if (player_2_score < 6)
        {
          player_2_score++;
          entities[SCORE_PLAYER_2].texture = scene_textures[player_2_score];

          audio_system->playSound(scene_sounds[2], nullptr, false, &audio_channel2);

          entities[PONG].position = vec2(0.f, 0.f);
          entities[PONG].collision.position = vec2(0.f, 0.f);
          pong_direction = vec2(GetRandomFloatInRange(-1.f, 1.f), GetRandomFloatInRange(-1.f, 1.f));
          pong_direction = vec2::normalize(pong_direction);

          hits /= 2;
        }
        else
        {
          gameplay_active = false;
          audio_channel0->stop();
          LoadScene(AssetPath("player_2_win.enscene").c_str());
          audio_system->playSound(scene_sounds[0], nullptr, false, &audio_channel0);
        }
      }

      if (AABBAABBColliding(entities[BOUNDARY_RIGHT].collision, entities[PONG].collision))
      {
        if (player_1_score < 6)
        {
          player_1_score++;
          entities[SCORE_PLAYER_1].texture = scene_textures[player_1_score];

          audio_system->playSound(scene_sounds[2], nullptr, false, &audio_channel2);

          entities[PONG].position = vec2(0.f, 0.f);
          entities[PONG].collision.position = vec2(0.f, 0.f);
          pong_direction = vec2(GetRandomFloatInRange(-1.f, 1.f), GetRandomFloatInRange(-1.f, 1.f));
          pong_direction = vec2::normalize(pong_direction);
      
          hits /= 2;
        }
        else
        {
          gameplay_active = false;
          audio_channel0->stop();
          LoadScene(AssetPath("player_1_win.enscene").c_str());
          audio_system->playSound(scene_sounds[0], nullptr, false, &audio_channel0);
        }
      }

      speed = 0.000001f + ((f32)hits * 0.00000005f);
      entities[PONG].position += speed * delta_time * pong_direction;
      entities[PONG].collision.position = entities[PONG].position;
    }

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
        glBindVertexArray(e.vao);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(f32), (void*)0);
        if (e.texture >= 0)
        {
          glActiveTexture(GL_TEXTURE0);
          glBindTexture(GL_TEXTURE_2D, e.texture);
        }
        else
        {
          glUniform4f(glGetUniformLocation(e.shader, "override_color"), e.color.x(), e.color.y(), e.color.z(), e.color.w());
        }
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
      }
    }

    glfwSwapBuffers(window);

    game_time = (f32)GetTimerValue(game_timer) / 1000.f;
    StopTimer(frame_timer);
    delta_time = frame_timer.time_delta / 1000.f;
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
