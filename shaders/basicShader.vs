#version 120

attribute vec3 position;
attribute vec3 normal;
attribute vec2 texCoord;

out vec2 texCoord0;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(position, 1.0);
    texCoord0 = texCoord;
}
