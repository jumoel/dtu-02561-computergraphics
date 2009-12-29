uniform samplerCube texMap;

void main()
{
  gl_FragColor = textureCube(texMap, gl_TexCoord[0].xyz);
}