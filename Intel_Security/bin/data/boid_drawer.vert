#version 120
uniform mat4 u_projection_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_model_matrix;
uniform mat3 u_orientation_matrix;
uniform mat3 u_normal_matrix;

attribute vec4 a_pos;
attribute vec2 a_tex;
attribute vec3 a_norm;


varying vec3 v_norm;
varying vec3 v_pos;
varying vec2 v_tex;

void main() {
	v_norm = u_normal_matrix * a_norm;
/// = u_projection_matrix * u_view_matrix  * u_model_matrix * a_pos;
	gl_Position = u_projection_matrix * u_view_matrix  * u_model_matrix * a_pos;
	v_pos = vec3(u_view_matrix * u_model_matrix * vec4(a_pos));
	v_tex = a_tex;
}