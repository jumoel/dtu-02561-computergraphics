uniform samplerCube texMap;
uniform sampler2D normalMap;

varying vec3 normal;
varying mat3 normalmat;
varying vec3 position;

uniform vec3 camera;
uniform float refraction;

varying float theta;
varying float phi;

varying vec3 X;
varying vec3 T;
varying vec3 B;

void main()
{
  vec2 coord = vec2(theta, phi);
  vec3 C = texture2D(normalMap, coord).xyz;
  vec3 V = ((C - 0.5) * 2.0);
  
  mat3 tbn = mat3(T, B, normal);
  
  vec3 n = normalize(normal);
  //vec3 n = normalize(tbn * V);
  vec3 i = -normalize(camera-position);
  
  vec3 rv = reflect(i, n);
  vec3 rf = refract(i, n, 1.0 / refraction);
  
  //float r = max(0.0, pow(dot(i, n), 2.0));  
  float rzero = 0.7;
  float r = rzero + (1.0-rzero) * pow(1.0-cos(dot(i,n)), 5.0);
  
  vec4 reflectcolor = textureCube(texMap, rv);
  vec4 refractcolor = textureCube(texMap, rf);
      
  gl_FragColor = r * refractcolor + (1.0 - r) * reflectcolor;
  //gl_FragColor = refractcolor;
}