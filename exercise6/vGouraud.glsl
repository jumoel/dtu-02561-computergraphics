/* Vertex shader */

uniform float time;

void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
