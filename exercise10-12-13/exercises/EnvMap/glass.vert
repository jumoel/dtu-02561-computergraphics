varying vec3 rv;
varying vec3 rf;
varying float r;

uniform vec3 camera;

void main()
{
    gl_Position = ftransform();
    
    vec3 position = vec3(gl_Vertex);
    vec3 normal   = gl_Normal;
    
    vec3 n = normalize(normal);
	vec3 i = -normalize(camera-position);
	
	rv = reflect(i, n);
	rf = refract(i, n, 1.0 / 2.04);
	
	r = max(0, pow(dot(i, n), 3));
	/*
	Water: 1.33
	Glass: 2.04
	
	http://www.robinwood.com/Catalog/Technical/Gen3DTuts/Gen3DPages/RefractionIndexList.html
	*/
}