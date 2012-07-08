uniform mat4 u_projection_matrix;
uniform mat4 u_modelview_matrix;
uniform mat4 u_model;

attribute vec4 a_pos;
attribute vec3 a_col;

varying vec3 v_col;

void main() {
	gl_Position = u_projection_matrix * u_modelview_matrix * u_model * a_pos;
	v_col = a_col;
}