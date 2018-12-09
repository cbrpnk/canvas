#version 120

uniform mat4 transform;

void main()
{
    gl_Position = gl_Vertex * transform;
}
