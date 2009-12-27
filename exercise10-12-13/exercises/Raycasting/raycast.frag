uniform sampler3D sh_texvol;
uniform vec3 camera;
float stepsize = 0.001;
varying vec3 position;
float iso = 0.5;

void main(void)
{
  vec3 delta_dir;//delta increment in the direction vector
  vec3 tex_pos = position;//texture position. Initially set to vertex position
  vec4 col_acc = vec4(0.0);//Accumulated color along the ray
  float alpha_acc = 0.0;//Accumulated alpha component of color
  float length_acc = 0.0;//length of the ray 
  vec4 col_sample; //Color value read from 3D texture
  float alpha_sample;
  
  float cube_ray_length;//maximum length of any ray to be traversed

  //TODO: march the ray along direction vector.
  
  gl_FragColor = col_acc;
}
