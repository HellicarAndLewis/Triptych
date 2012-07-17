varying vec2 v_tex;
uniform sampler2D u_texture;
uniform sampler2D u_flow;
uniform float u_time;
uniform vec4 u_trail_color;
void main() {
	vec4 flowc = texture2D(u_flow, vec2(v_tex.x +u_time, v_tex.y));
	vec2 flow_tex = (flowc.rg * 2.0 - 1.0);
	vec2 new_tex = vec2(v_tex.x + flow_tex.x, v_tex.y + flow_tex.y);
	
	vec4 texc = texture2D(u_texture, v_tex);
	//texc.rgb = texc.rgb * vec3(1.0, 0.0, 0.0);
	gl_FragColor = texc * u_trail_color;
	//gl_FragColor.a = u_trail_color.a;
	//gl_FragColor.rgba = vec4(1.0, 1.0, 1.0, 1.0);
}