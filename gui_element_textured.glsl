#shader vertex
#version 330 core

layout (location = 0) in vec2 position;

out vec2 tex_coords;

uniform mat4 transform;

void main()
{
  gl_Position = transform * vec4(position, 0.f, 1.f);
  tex_coords = vec2((position.x + 1.f) / 2.f, 1.f - (position.y + 1.f) / 2.f);
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
