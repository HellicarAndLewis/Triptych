uniform sampler2D u_texture;

varying vec3 v_norm;
varying vec3 v_pos;
varying vec2 v_tex;

void main() {
	vec4 texc = texture2D(u_texture, vec2(v_tex.x, 1.0-v_tex.y));
	vec3 light_pos = vec3(0.0,-15.0,-14.0);
	vec3 light_dir = (light_pos - v_pos);
	float ndotl = max(dot(v_norm, light_dir), 0.0);
	vec3 diffuse_col = vec3(0.4,0.3,0.1);
	gl_FragColor = vec4(1.0, 0.0, 0.2,0.5);
	gl_FragColor.rgb = 0.5 + v_norm * 0.5;
	gl_FragColor.a = 1.0;
	vec3 light_col = vec3(0.1,0.1,0.1);
//	gl_FragColor.rgb = texc.rgb * light_col * ndotl;
	gl_FragColor.rgb = mix(texc.rgb * light_col * ndotl, v_norm, 0.3);
	//gl_FragColor.rgb = vec3(1.0, 0.0, 0.0) * ndotl;
	//gl_FragColor.rgb = v_norm;
}