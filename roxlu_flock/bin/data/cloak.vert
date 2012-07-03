uniform mat4 u_projection_matrix;
uniform mat4 u_modelview_matrix;
uniform mat3 u_normal_matrix;

attribute vec4 a_pos;
attribute vec4 a_col;
attribute vec3 a_norm;

varying vec4 v_col;
varying vec3 v_norm;
varying vec3 v_trans_norm;

void main() {
	gl_Position = u_projection_matrix * u_modelview_matrix * a_pos;
	gl_Position.z = 0.0;
	v_col = a_col;
	v_norm = a_norm;
	v_trans_norm = u_normal_matrix * v_norm;
	v_trans_norm = v_norm;
}