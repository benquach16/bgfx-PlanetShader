$input v_pos, v_view, v_normal, v_color0


#include "./common/common.sh"


vec3 lightPosition = vec3(0.0, 0.0, -5.0);
	
void main()
{
	vec3 normal = normalize(v_normal);
	vec3 viewDirection = -normalize(v_view);
	vec3 vertexPosition = v_pos;

	vec3 lightDirection = normalize(lightPosition - vertexPosition);
	//viewDirection = normalize(viewDirection - vertexPosition);

	//apply lamberts cosine law
	vec4 col = vec4(0.5,1.0, 0.5, 1.0);
	vec4 ambient = vec4(0.01,0.01,0.01,1.0);
	col = col * max(dot(normal, lightDirection), 0.0);

	float rim = 1.0 - dot(viewDirection, normal);
	rim += 0.2;

	//specular
	float specDot = dot(normal, lightDirection);
	vec3 R = lightDirection - 2.0*specDot*normal;
	float viewDot = dot(R, viewDirection);
	
	gl_FragColor = ambient + col + pow(rim, 8.0) * vec4(0.3,0.6,1.0,1.0);

}
