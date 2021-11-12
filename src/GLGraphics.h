#pragma once

#define GLEW_STATIC

#include "glew/glew.h"
#include "Core.h"
#include "unordered_map.h"
#include "stb/stb_image.h"


/// Graphics API Reference
////// u32 LoadGLTexture(const char* texture);
////// u32 LoadGLShader(const char* shader);

u32 quad_indices[] = { 0, 1, 3, 1, 2, 3 };

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
