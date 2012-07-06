#version 120 
varying vec4 v_col;

void main() {
	gl_FragColor.rgb = v_col.rgb;
	gl_FragColor.a = 1.0;	
}