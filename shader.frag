#version 120

// TODO This should not be hardcoded
vec2 iResolution = vec2(640, 480);

/*
 * Sdf functions by Inigo Quilez
 * http://iquilezles.org/www/articles/distfunctions2d/distfunctions2d.htm
 */

vec3 outputColor = vec3(0.);
// Acumulate the output of sdf function until draw command
float stencilBuffer = 0.;
// While paths are constructed they are summed into this buffer
float pathBuffer = 0.;

vec2 uv = vec2(0., 0.);
#define UV_STACK_SIZE 10
vec2 uvStack[UV_STACK_SIZE];
vec4 fillColor = vec4(1.);
float strokeWidth = 0.001;

vec2 pathFirstPoint = vec2(0.);
vec2 pathLastPoint = vec2(0.);


/////////////////////////// Base Drawing //////////////////////////

#define ANTIALIAS(a) 1.-smoothstep(0., .0025, (a))

#define ADDFILL(f) stencilBuffer += ANTIALIAS((f))
#define SUBFILL(f) stencilBuffer -= ANTIALIAS((f))
#define INTERFILL(f) stencilBuffer = min(stencilBuffer, ANTIALIAS((f)))

#define ADDSTROKE(f) stencilBuffer += ANTIALIAS(abs((f)) - strokeWidth)
#define SUBSTROKE(f) stencilBuffer -= ANTIALIAS(abs((f)) - strokeWidth)
#define INTERSTROKE(f) stencilBuffer = min(stencilBuffer, ANTIALIAS(abs((f)) - strokeWidth))


// Draw mixes the stencil into the outputColor
void draw() {
    outputColor = mix((outputColor), fillColor.rgb,
                      clamp(stencilBuffer, 0., 1.) * fillColor.a);
    stencilBuffer = 0.;
}

//////////////////////////// Trasnforms ///////////////////////////


void rotate(float a) {
    uv *= mat2(cos((a)), sin((a)), -sin((a)), cos((a)));
}

void scale(float f)
{
    uv /= f;
}

void translate(float x, float y)
{
    uv -= vec2((x), (y));
}

#define save(i) uvStack[i] = uv
#define restore(i) uv = uvStack[i]

/////////////////////////////// Style /////////////////////////////

void linearGrad(vec4 c1, vec4 c2)
{
    // By default the gradient goes from the bottom to the top.
    // If you want something else, use the transform functions first.
    fillColor = mix(c1, c2, uv.y);
}

void radialGrad(vec4 c1, vec4 c2)
{
    fillColor = mix(c1, c2, length(uv));
}

void fill(vec4 col)
{
    fillColor = col;
}

void stroke(float w)
{
    strokeWidth = w;
}


//////////////////////////// Triangle /////////////////////////////

float tri(vec2 p0, vec2 p1, vec2 p2 )
{
    vec2 e0 = p1-p0, e1 = p2-p1, e2 = p0-p2;
    vec2 v0 = uv -p0, v1 = uv -p1, v2 = uv -p2;

    vec2 pq0 = v0 - e0*clamp( dot(v0,e0)/dot(e0,e0), 0.0, 1.0 );
    vec2 pq1 = v1 - e1*clamp( dot(v1,e1)/dot(e1,e1), 0.0, 1.0 );
    vec2 pq2 = v2 - e2*clamp( dot(v2,e2)/dot(e2,e2), 0.0, 1.0 );
    
    float s = sign( e0.x*e2.y - e0.y*e2.x );
    vec2 d = min(min(vec2(dot(pq0,pq0), s*(v0.x*e0.y-v0.y*e0.x)),
                     vec2(dot(pq1,pq1), s*(v1.x*e1.y-v1.y*e1.x))),
                     vec2(dot(pq2,pq2), s*(v2.x*e2.y-v2.y*e2.x)));

    return -sqrt(d.x)*sign(d.y);
}


//////////////////////////// Rect /////////////////////////////

float rect(vec2 p, vec2 b )
{
    vec2 d = abs(uv-p)-b;
    return length(max(d,vec2(0))) + min(max(d.x,d.y),0.0);
}


///////////////////////////// Line //////////////////////////////

float line(vec2 a, vec2 b )
{
    vec2 pa = uv-a, ba = b-a;
    float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
    return length( pa - ba*h );
}

void beginPath(vec2 p)
{
    pathBuffer = 0.;
    pathFirstPoint = p;
    pathLastPoint = p;
}

void lineTo(vec2 p)
{
    pathBuffer += ANTIALIAS(abs(line(pathLastPoint, p)) - strokeWidth);
    pathLastPoint = p;
}

void closePath()
{
    pathBuffer += ANTIALIAS(abs(line(pathLastPoint, pathFirstPoint)) - strokeWidth);
    pathLastPoint = pathFirstPoint;
}

void addPath()
{
    stencilBuffer += pathBuffer;
}

void subPath()
{
    stencilBuffer -= pathBuffer;
}

void interPath()
{
    stencilBuffer = min(stencilBuffer, pathBuffer);
}


//////////////////////////// Circle /////////////////////////////

float circle(vec2 pos, float radius)
{
    return length(uv-pos)-radius;
}


//////////////////////////// Main /////////////////////////////

void main()
{
    uv = gl_FragCoord.xy/iResolution.xy;
    uv.x *= iResolution.x/iResolution.y;
    
    // Stroke
    stroke(.003);
    
    // Triangle
    fill(vec4(1., 0., 0., 1.));
    ADDFILL(tri(vec2(.1, .3), vec2(.3, .7), vec2(1.5, .3)));
    INTERFILL(circle(vec2(.1, .34), .3));
    SUBSTROKE(tri(vec2(.2, .34), vec2(.3, .7), vec2(1.5, .3)));
    SUBSTROKE(line(vec2(.3, .4), vec2(.5, .5)));
    draw();
    
    // Circle
    save(0);
    fill(vec4(0., 1., 0., .5));
    save(1);
    translate(.25, .5);
    scale(.1);
    rotate(.2);
    radialGrad(vec4(1., 0., 0. ,1.), vec4(0., 1., 0., 1));
    restore(1);
    ADDFILL(circle(vec2(.25, .5), .08));
    SUBSTROKE(circle(vec2(.25, .5), .03));
    draw();
    restore(0);
    
    // Rect
    save(0);
    rotate(.3);
    scale(.5);
    translate(.4, .6);
    fill(vec4(0., 0., 1., .5));
    ADDFILL(rect(vec2(.0, .0), vec2(.1, .1)));
    SUBSTROKE(rect(vec2(.0, .0), vec2(.05, .1)));
    draw();
    restore(0);
    
    // Line
    fill(vec4(1., 1., 0., 1.));
    ADDSTROKE(line(vec2(.4, .3), vec2(.5, .5)));
    draw();
    
    // Path
    stroke(0.0005);
    beginPath(vec2(.1, .3));
    lineTo(vec2(.2, .4));
    lineTo(vec2(.1, .2));
    lineTo(vec2(.5, .4));
    lineTo(vec2(.8, .4));
    addPath();
    draw();
    
    // Output to screen
    gl_FragColor = vec4(outputColor, 1.);
}
