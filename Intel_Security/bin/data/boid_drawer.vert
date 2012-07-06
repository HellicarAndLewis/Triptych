uniform mat4 u_projection_matrix;
uniform mat4 u_modelview_matrix;

attribute vec4 a_pos;

void main() {
	gl_Position = u_projection_matrix * u_modelview_matrix * a_pos;
}