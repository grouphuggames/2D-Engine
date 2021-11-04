#shader vertex
#version 330 core

layout (location = 0) in vec2 position;

out vec2 tex_coords;

uniform mat4 transform;
uniform int anim_frame;
uniform float anim_flip_val;    // set to 1 if not flipped, -1 if flipped

void main()
{
  float anim_x_offset;
  float anim_y_offset;

  if (anim_frame < 5)
  {
    anim_x_offset = anim_frame;
    anim_y_offset = 0.f;
  }
  else
  {
    anim_x_offset = anim_frame - 5.f;
    anim_y_offset = 1.f;
  }

  gl_Position = transform * vec4(position, 0.f, 1.f);
  tex_coords = vec2(anim_flip_val * ((position.x + 1.f) / 2.f) / 5.f + (anim_x_offset / 5.f), (1.f - (position.y + 1.f) / 2.f) / 2.f - (anim_y_offset / 2.f));
}

#shader fragment
#version 330 core

in vec2 tex_coords;

out vec4 out_color;

uniform sampler2D _texture;

void main()
{
  out_color = texture(_texture, tex_coords);
}
