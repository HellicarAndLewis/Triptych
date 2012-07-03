varying vec4 v_col;
varying vec3 v_norm;
varying vec3 v_trans_norm;

void main() {
	gl_FragColor = v_col;
	gl_FragColor.rgb = v_trans_norm;
	
	
	// DIRECTIONAL LIGHTING
	vec3 light_dir = vec3(0,-5,-10.1);
    float ndotl = dot(v_trans_norm, light_dir);
    float light_w = max(ndotl, 0.0);
	vec3 ambient_col = vec3(1.0, 0.7,0.2);
//	vec3 ambient_col = vec3(0.0, 1.0,0.0);
	vec3 dir_col = vec3(0.6, 0.6, 0.7);
//	vec3 dir_col = vec3(1.0, 0.1, 0.1);
    vec3 light_col = ambient_col + dir_col * light_w;
//    light_col = dir_col * light_w;
   // gl_FragColor = vec4( v_trans_norm + light_col, 1.0 );
	gl_FragColor.rgb = v_trans_norm;

	gl_FragColor.rgb = light_col;
	gl_FragColor.rgb = mix(light_col, v_trans_norm,0.8);
	gl_FragColor.a = 1.0;
	//gl_FragColor = vec4(1.0, 0.3,0.3,1.0);
}