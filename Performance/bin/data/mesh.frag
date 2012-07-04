#version 120

varying vec4 color;
uniform int layer;

void main()
{

	vec4 c = color;
	if(layer==0) {
		c = sqrt(c);
	} else {
		c.a = 0.5;
		if(layer==1) {
			c.r = sqrt(c.r);
			c.g *= c.g;
			c.b *= c.b;
		} else if(layer==2) {
			c.g = sqrt(c.g);
			c.r *= c.r;
			c.b *= c.b;
		} else if(layer==3) {
			c.b = sqrt(c.b);
			c.r *= c.r;
			c.g *= c.g;
		}
	}
	gl_FragColor = c;
}