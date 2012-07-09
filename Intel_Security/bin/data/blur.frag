#version 120 
uniform sampler2D u_texture;
uniform int u_mode;
uniform float u_height;
uniform float u_width;
varying vec2 vtex;

void main() {
	
	// mac 10.6, array initializers not supported
	float offset[3];
	float weight[3];
	
	offset[0] = 0.0;
	offset[1] = 1.3846153846;
	offset[2] = 3.2307692308;
	weight[0] = 0.2270270270;
	weight[1] = 0.3162162162;
	weight[2] = 0.0702702703;
	
	vec3 col = texture2D(u_texture, vtex).rgb * weight[0];
	
	if(u_mode == 0) {
		float dy = 1.0f/u_height;
		for(int i=1; i<3; i++) {
			col += texture2D(u_texture, vtex + vec2(0.0, offset[i]) * dy).rgb * weight[i];
			col += texture2D(u_texture, vtex - vec2(0.0, offset[i]) * dy).rgb * weight[i];
		}
   	}
	else if(u_mode == 1) {
		float dx = 1.0f/u_width;
		
		
		for(int i=1; i<3; i++) {
			col += texture2D(u_texture, vtex + vec2(offset[i]) * dx, 0.0).rgb * weight[i];
			col += texture2D(u_texture, vtex - vec2(offset[i]) * dx, 0.0).rgb * weight[i];
		}
	}
	//gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0) * col;
	gl_FragColor.rgb = vec3(1.0, 0.0, 0.0);
	gl_FragColor.a = 1.0;
	//gl_FragColor.rgb = col;
}