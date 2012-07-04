varying vec2 v_tex;
uniform vec2 u_particles[20];
uniform sampler2D u_texture;
uniform float u_width;
uniform float u_height;

void main() {
	vec2 tex = v_tex;
	float dx = 0.0;
	float dy = 0.0;
	float tx = 0.0;
	float ty = 0.0;
	vec2 resolution = vec2(u_width, u_height);
	vec2 p = -1.0 + 2.0 * gl_FragCoord.xy / resolution.xy;
	vec2 dir = u_particles[0] - p;
	float dist = sqrt(dot(dir, dir));
	
	tex.x = 1.0 + cos(dist) * 0.5;
	tex.y = 1.0 + sin(dist) * 0.6;
	
	vec4 col = texture2D(u_texture, tex);
	gl_FragColor = vec4(1.0, 0.1, 0.1,1.0);
	gl_FragColor.r = u_particles[0].x;
	gl_FragColor = col;
	gl_FragColor.a = 1.0;
	
	
	
	vec2 cpos = vec2(0.5, 0.5);
	float len = length(cpos);
	
    vec4 cols = texture2D(u_texture,cpos);
    gl_FragColor = cols;
	gl_FragColor.a = 1.0;
}