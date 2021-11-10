#shader vertex
#version 330 core

layout (location = 0) in vec2 _position;
layout (location = 1) in vec4 _color;
layout (location = 2) in vec2 _tex_coords;
layout (location = 3) in float _tex_index;

out vec2 tex_coords;
out float tex_index;

uniform mat4 transform;

void main()
{
  gl_Position = transform * vec4(_position, 0.f, 1.f);
  tex_coords = _tex_coords;
  tex_index = _tex_index;
}

#shader fragment
#version 330 core

in vec2 tex_coords;
in float tex_index;

out vec4 out_color;

uniform sampler2D textures[5];

void main()
{
  int index = int(tex_index);
  out_color = texture(textures[index], tex_coords);
}
