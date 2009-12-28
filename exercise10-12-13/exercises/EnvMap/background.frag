/*
void main(void)
{
	gl_FragColor = vec4(0,0,1,1);
}
*/

varying vec3 R;
uniform samplerCube texMap;

void main()
{
     vec4 texColor = textureCube(texMap, R);

    gl_FragColor = texColor;
}
