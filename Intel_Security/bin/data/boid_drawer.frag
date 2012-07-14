uniform sampler2D u_texture;
uniform float u_normal_mix;
uniform float u_specular_component;
varying vec3 v_norm;
varying vec3 v_pos;
varying vec2 v_tex;

uniform float u_fog_min;
uniform float u_fog_max;

void main() {
	vec4 texc = texture2D(u_texture, vec2(v_tex.x, 1.0-v_tex.y));
	vec3 light_pos = vec3(0.0,-5.0,-5.0);
	vec3 light_dir = normalize(light_pos - v_pos);
	vec3 light_e = normalize(-v_pos);
	vec3 light_r = -reflect(light_dir, light_e);
	vec3 spec_color = vec3(1.0, 1.0, 1.0) * pow(max(dot(light_e, light_r), 0.0), u_specular_component);
	float ndotl = max(dot(v_norm, light_dir), 0.0);

	// begin fog
	float dist = abs(v_pos.z);
	float fog_factor = (u_fog_max - dist) / (u_fog_max - u_fog_min);
	fog_factor = clamp(fog_factor, 0.0, 1.0);
	vec3 fog_color = vec3(0.0, 0.0, 0.0);
	// end fog

	gl_FragColor.a = 1.0;
	vec3 light_col = vec3(0.5,0.0,0.0);
	gl_FragColor.rgb =  mix(v_norm,  texc.rgb * (ndotl+0.1) + ndotl * light_col, u_normal_mix) +spec_color;
	//gl_FragColor.rgb = vec3(0.0, 1.0, 0.0);
	gl_FragColor.rgb = mix(fog_color, gl_FragColor.rgb, fog_factor);
//	gl_FragColor.rgb = fog_color * fog_factor;

	
	
//	gl_FragColor.rgb = v_norm * 0.3 + spec_color + texc.rgb + vec3(1.0,0.3,0.1) * ndotl;
//	gl_FragColor.rgb = spec_color + texc.rgb + mix( light_col * ndotl, v_norm, 0.1);
	//gl_FragColor.rgb = spec_color;
	//gl_FragColor.rgb = vec3(1.0, 0.0, 0.0) * ndotl;
//	gl_FragColor.rgb = v_norm;
}