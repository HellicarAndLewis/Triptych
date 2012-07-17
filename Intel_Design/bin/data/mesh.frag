#version 120

varying vec4 color;
uniform int layer;

uniform float colorPower;
uniform float atten;
uniform float channelAlpha;

void main()
{

	vec4 c = color;
	if(layer==0) {
		c = sqrt(c);
	} else {
		c.a = channelAlpha;
		if(layer==1) {
			c.r = pow(c.r, colorPower);
			c.g *= c.g*atten;
			c.b *= c.b*atten;
		} else if(layer==2) {
			c.g = sqrt(c.g);
			c.r *= c.r*atten;
			c.b *= c.b*atten;
		} else if(layer==3) {
			c.b = sqrt(c.b);
			c.r *= c.r*atten;
			c.g *= c.g*atten;
		}
	}
	gl_FragColor = c;
}