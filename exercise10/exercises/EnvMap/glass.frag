uniform vec3 camera;

const float pi = 3.14159;

//useful debug function
vec3 vec2col(in vec3 v)
{
	return (v+1.) * 0.5;
}

void main(void)
{
	gl_FragColor = vec4(0,1,0,1);
}