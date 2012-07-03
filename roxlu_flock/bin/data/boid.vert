uniform mat4 u_projection_matrix;
uniform mat4 u_modelview_matrix;


attribute vec4 a_pos;
attribute vec2 a_tex;

varying vec2 v_tex;

void main() {
	gl_Position = u_projection_matrix * u_modelview_matrix * a_pos;
	v_tex = a_tex;
}