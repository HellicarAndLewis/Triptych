uniform mat4 u_projection_matrix;
uniform mat4 u_modelview_matrix;


attribute vec4 a_pos;
attribute vec4 a_col;

varying vec4 v_col;

void main() {
	gl_Position = u_projection_matrix * u_modelview_matrix * a_pos;
	v_col = a_col;
}