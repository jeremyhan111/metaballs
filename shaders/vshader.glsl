#version 410

in vec2 vPosition;
uniform mat4 mview;
uniform vec2 displacement;

void
main()
{   
	vec4 pos = vec4(vPosition+displacement, 0., 1.);
    gl_Position = mview*pos;
}
