varying vec3 position;
varying vec3 normal;

void main()
{
    gl_Position = ftransform();
    
    position = gl_Vertex;
    normal   = gl_Normal;    
}