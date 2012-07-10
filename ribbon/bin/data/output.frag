#version 120


#define KERNEL_SIZE 25

uniform sampler2DRect tDiffuse;
uniform vec2 uImageIncrement;



void main() {
	
	vec2 imageCoord = gl_TexCoord[0].st-uImageIncrement*12.0;
	
	
	vec4 sum = vec4( 0.0, 0.0, 0.0, 0.0 );
	float t = 1.5;
	
	sum += texture2DRect( tDiffuse, imageCoord ) * 0.001110*t;
	imageCoord += uImageIncrement;

	
	sum += texture2DRect( tDiffuse, imageCoord ) * 0.002277*t;
	imageCoord += uImageIncrement;
	
	sum += texture2DRect( tDiffuse, imageCoord ) * 0.004390*t;
	imageCoord += uImageIncrement;
	
	sum += texture2DRect( tDiffuse, imageCoord ) * 0.007949*t;
	imageCoord += uImageIncrement;
	
	sum += texture2DRect( tDiffuse, imageCoord ) * 0.013521*t;
	imageCoord += uImageIncrement;
	
	sum += texture2DRect( tDiffuse, imageCoord ) * 0.021607*t;
	imageCoord += uImageIncrement;
	
	sum += texture2DRect( tDiffuse, imageCoord ) * 0.032436*t;
	imageCoord += uImageIncrement;
	
	sum += texture2DRect( tDiffuse, imageCoord ) * 0.045741*t;
	imageCoord += uImageIncrement;
	
	sum += texture2DRect( tDiffuse, imageCoord ) * 0.060597*t;
	imageCoord += uImageIncrement;
	
	sum += texture2DRect( tDiffuse, imageCoord ) * 0.075415*t;
	imageCoord += uImageIncrement;
	
	sum += texture2DRect( tDiffuse, imageCoord ) * 0.088169*t;
	imageCoord += uImageIncrement;
	
	sum += texture2DRect( tDiffuse, imageCoord ) * 0.096835*t;
	imageCoord += uImageIncrement;
	
	sum += texture2DRect( tDiffuse, imageCoord ) * 0.099908*t;
	imageCoord += uImageIncrement;
	
	sum += texture2DRect( tDiffuse, imageCoord ) * 0.096835*t;
	imageCoord += uImageIncrement;
	
	sum += texture2DRect( tDiffuse, imageCoord ) * 0.088169*t;
	imageCoord += uImageIncrement;
	
	sum += texture2DRect( tDiffuse, imageCoord ) * 0.075415*t;
	imageCoord += uImageIncrement;
	
	sum += texture2DRect( tDiffuse, imageCoord ) * 0.060597*t;
	imageCoord += uImageIncrement;
	
	sum += texture2DRect( tDiffuse, imageCoord ) * 0.045741*t;
	imageCoord += uImageIncrement;
	
	sum += texture2DRect( tDiffuse, imageCoord ) * 0.032436*t;
	imageCoord += uImageIncrement;
	
	sum += texture2DRect( tDiffuse, imageCoord ) * 0.021607*t;
	imageCoord += uImageIncrement;
	
	sum += texture2DRect( tDiffuse, imageCoord ) * 0.013521*t;
	imageCoord += uImageIncrement;
	
	sum += texture2DRect( tDiffuse, imageCoord ) * 0.007949*t;
	imageCoord += uImageIncrement;
	
	sum += texture2DRect( tDiffuse, imageCoord ) * 0.004390*t;
	imageCoord += uImageIncrement;
	
	sum += texture2DRect( tDiffuse, imageCoord ) * 0.002277*t;
	imageCoord += uImageIncrement;
	
	sum += texture2DRect( tDiffuse, imageCoord ) * 0.001110*t;
	
	gl_FragColor = sum;
	if(sum.r>0.0) gl_FragColor.a = 1.0;
}