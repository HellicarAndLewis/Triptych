#version 120 
uniform mat4 u_modelview_matrix;
uniform mat4 u_projection_matrix;
attribute vec4 pos;
attribute vec2 tex;

varying vec2 vtex;

void main() {
	gl_Position = u_projection_matrix * u_modelview_matrix * pos;
	vtex = tex;
}