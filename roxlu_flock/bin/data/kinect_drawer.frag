varying vec3 v_col;
void main() {

	gl_FragColor = vec4(1.0, 0.1, 0.3,1.0);
	gl_FragColor.rgb = v_col;
	gl_FragColor.a  = 1.0;
}