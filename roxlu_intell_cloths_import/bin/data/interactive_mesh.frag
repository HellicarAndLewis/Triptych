#version 120 
varying vec3 v_norm;
uniform int u_mode;
void main() {
	
	if(u_mode == 0) {
		gl_FragColor.rgb = v_norm;
	}
	else {
		gl_FragColor.rgb = vec3(0.0, 0.0, 0.0);
	}
}