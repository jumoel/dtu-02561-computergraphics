void main(void)
{
	gl_Position = ftransform();
	gl_TexCoord[0].xyz = gl_Vertex.xyz;
}