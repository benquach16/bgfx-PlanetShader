$input v_pos, v_view, v_normal, v_color0, v_texcoord0


#include "./common/common.sh"


vec3 lightPosition = vec3(-5.0, 5.0, -5.0);


SAMPLER2D(s_planet_texture, 0);

void main()
{
	vec3 normal = normalize(v_normal);
	vec3 viewDirection = -normalize(v_view);
	vec3 vertexPosition = v_pos;

	vec3 lightDirection = normalize(lightPosition - vertexPosition);
	//viewDirection = normalize(viewDirection - vertexPosition);

	//apply lamberts cosine law
	vec4 col = vec4(0.3,0.7, 0.4, 1.0);
	vec4 ambient = vec4(0.01,0.01,0.01,1.0);
	col = col * max(dot(normal, lightDirection), 0.0);


	//rim shader
	float rim = 1.0 - dot(viewDirection, normal);
	rim+=0.1;
	//take into account light direction
	float rimLight = dot(lightDirection, normal);
	float f_rim = pow(rim, 8.0);
	f_rim *= (rimLight*2);

	//speculards
	vec3 reflectVec = normalize(-reflect(lightDirection, normal));
	float spec = 0.5* pow(max(dot(reflectVec, viewDirection), 0.0), 5.0);
	vec4 tex = texture2D(s_planet_texture, v_texcoord0.xy);
	gl_FragColor = ambient + tex+ spec + f_rim * vec4(0.3,0.6,1.0,1.0);
	//ogl_FragColor = texture2D(s_planet_texture, v_texcoord0.xy);
	gl_FragColor.w = 1.0;
}
