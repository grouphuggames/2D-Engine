#pragma once

#include "Core.h"
#include "vec2.h"


struct Vertex
{
  vec2 position;
};

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
