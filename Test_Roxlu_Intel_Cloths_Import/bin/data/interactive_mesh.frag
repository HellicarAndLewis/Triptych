#version 120 
varying vec3 v_norm;
uniform int u_mode;
void main() {
	
	if(u_mode == 0) {
		gl_FragColor.rgb = 0.5 + (v_norm * 0.5);
	}
	else {
		gl_FragColor.rgb = vec3(0.0, 0.0, 0.0);
	}
}