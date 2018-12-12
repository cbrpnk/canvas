#version 120

uniform vec3 fillColor;

uniform int hasTexture;
uniform sampler2D tex;

void main()
{
    vec2 tCoord = vec2(gl_FragCoord.x/192, -gl_FragCoord.y/108);
    vec4 col;
    
    if(hasTexture == 1) {
        col = texture2D(tex, tCoord);
    } else {
        col = vec4(1, 0, 0, 1);
    }
    
    gl_FragColor = col;
}
