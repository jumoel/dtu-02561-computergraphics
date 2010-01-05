uniform samplerCube texMap;

varying vec3 rv;
varying vec3 rf;
varying float r;

void main()
{
  vec4 reflectcolor = textureCube(texMap, rv);
  vec4 refractcolor = textureCube(texMap, rf);
      
  gl_FragColor = r * reflectcolor + (1.0 - r) * refractcolor;
}