varying vec3 position;

void main(void)
{
	gl_Position = ftransform();
	//TODO: Pass the object space vertex position.
}
