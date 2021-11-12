#pragma once

#include "Core.h"
#include "vector.h"
#include "GLGraphics.h"


en::vector<Entity> entities;
en::vector<vec2> original_scales;

//en::vector<std::string> LoadSceneSelector()
//{
//  en::vector<std::string> result;
//  
//  for (const auto& file : std::filesystem::directory_iterator(AssetPath("")))
//  {
//    std::string filepath = file.path().string();
//    
//    if (filepath.find(".enscene") != std::string::npos)
//    {
//      result.PushBack(filepath);
//    }
//  }
//
//  return result;
//}

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
