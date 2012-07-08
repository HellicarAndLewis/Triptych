varying vec2 v_tex;
uniform sampler2D u_texture;
uniform sampler2D u_flow;
uniform float u_time;

void main() {
	vec4 flowc = texture2D(u_flow, vec2(v_tex.x +u_time*0.1, v_tex.y));
	vec2 flow_tex = (flowc.rg * 2.0 - 1.0);
	vec2 new_tex = vec2(v_tex.x + flow_tex.x, v_tex.y + flow_tex.y);
	
	vec4 texc = texture2D(u_texture, new_tex);
	gl_FragColor = texc;
}