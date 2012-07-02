#version 120

varying vec4 diffuse,ambientGlobal, ambient;
varying vec3 normal,lightDir,halfVector;
varying float dist;
uniform sampler2DRect tex;
uniform float amt;
void main()
{
	vec3 n,halfV,viewV,ldir;
	float NdotL,NdotHV;
	vec4 color = ambientGlobal;
	float att;
	/* a fragment shader can't write a varying variable, hence we need

	a new variable to store the normalized interpolated normal */
	n = normalize(normal);
	/* compute the dot product between normal and normalized lightdir */

	float quadAtt = 0.4;
	float linAtt = 0.4;
	NdotL = max(dot(n,normalize(lightDir)),0.0);
//	NdotL = abs(dot(n,normalize(lightDir)));
	if (NdotL > 0.0) {

		att = 1.0 / (gl_LightSource[0].constantAttenuation +
				linAtt * dist +
				quadAtt * dist * dist);
		color += att * (diffuse * NdotL + ambient);
		halfV = normalize(halfVector);

		NdotHV = max(dot(n,halfV),0.0);
		/*color += att * gl_FrontMaterial.specular * gl_LightSource[0].specular *
						pow(NdotHV,gl_FrontMaterial.shininess);*/
	}
	
	
	gl_FragColor = (1.0-amt)*texture2DRect(tex, gl_TexCoord[0].xy) + (amt)*color;
}