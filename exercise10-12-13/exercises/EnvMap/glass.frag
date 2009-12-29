uniform vec3 camera;

uniform samplerCube texMap;

varying vec3 position;
varying vec3 normal;

void main()
{
  vec3 n = normalize(normal);
  vec3 i = -normalize(camera-position);
  
  vec3 rv = reflect(i, n);
  
  gl_FragColor = textureCube(texMap, rv);
}