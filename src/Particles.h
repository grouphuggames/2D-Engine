#pragma once

#include "Core.h"
#include "vec2.h"
#include "vec4.h"


struct ParticleVertex
{
  vec2 position;
  vec4 color;
  vec2 texture_coords;
  f32 texture_index;
};

const s32 MAX_PARTICLES = 5000;
s32 particle_textures[5];

struct
{
  u32 vao, vbo, ebo;
  en::vector<vec2> position;
  en::vector<vec2> scale;
  en::vector<vec4> color;
} particles;
