varying vec3 normal;
varying mat3 normalmat;
varying vec3 position;

uniform vec3 camera;

varying float theta;
varying float phi;

varying vec3 T;
varying vec3 B;
varying vec3 X;

uniform sampler2D normalMap;

void main()
{
    gl_Position = ftransform();
    
    position = vec3(gl_Vertex);
    normal   = gl_Normal;
    normalmat = gl_NormalMatrix;
    
    float PI = 3.14159265358979323846264;
    
    // This works because the sphere is centered around the origin and the radius is 1
    theta = acos(gl_Vertex.z);  // [0, pi]
	phi = atan(gl_Vertex.y, gl_Vertex.x); // [-pi, pi]

	X = (vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta)));
	T = (vec3(cos(theta) * cos(phi), cos(theta) * sin(phi), -sin(theta)));
	B = (vec3(-sin(theta) * sin(phi), sin(theta) * cos(phi), 0));
	
	// Normalmap is [0,1]
	theta = (theta / PI);
	phi = (phi + PI) / (2.0 * PI);
}