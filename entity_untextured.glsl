#shader vertex
#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec4 color;

out vec4 _color;

uniform mat4 transform;

void main()
{
  gl_Position = transform * vec4(position, 0.f, 1.f);
  _color = color;
}

#shader fragment
#version 330 core

in vec4 _color;

out vec4 out_color;

void main()
{
  out_color = _color;
}
