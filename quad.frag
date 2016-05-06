uniform float time;
in float intensity;


float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453 + time);
}

void main(void)
{
    //gl_FragColor = gl_Color;
    float factor;
    vec4 color;

    if (intensity > 0.95)
	    factor = 0.95;
    else if (intensity > 0.5)
	    factor = 0.7;
    else if (intensity > 0.25)
	    factor = 0.5;
    else
	    factor = 0.3;

    gl_FragColor = max( gl_Color * 0.3, gl_Color * intensity);

    //gl_FragColor = gl_Color * intensity;
    //gl_FragColor = vec4(rand(vec2(0.5, 1.0)), 0.0, 0.0, 1.0);
    //gl_FragColor = vec4(rand(vec2(1.0,1.0)), rand(vec2(0.0,1.0)), rand(vec2(1.0,0.0)), 1.0);
}
