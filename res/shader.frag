#version 120

uniform vec3 fillColor;
uniform sampler2D tex;

void main()
{
    vec2 tCoord = vec2(gl_FragCoord.x/192, -gl_FragCoord.y/108);
    vec4 col = texture2D(tex, tCoord);
    //gl_FragColor = col;
    gl_FragColor = vec4(1., 0., 0., 1.);
}
