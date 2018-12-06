#version 120

uniform vec3 fillColor;

void main()
{
    gl_FragColor = vec4(fillColor, 1.);
}
