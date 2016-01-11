$input v_pos, v_view, v_normal, v_color0


#include "./common/common.sh"

float fresnel(float dotProduct, float bias, float power)
{
	float facing = 1.0 - dotProduct;
	
	return bias + (1.0 - bias) * pow(facing, power);
}


void main()
{
	vec3 normal = normalize(v_normal);
	vec3 viewPosition = normalize(v_view);
	vec3 vertexPosition = v_pos;
	vec3 lightPosition = vec3(0.0, 0.0, -5.0);
	vec3 lightDirection = normalize(lightPosition - vertexPosition);
	vec3 viewDirection = normalize(viewPosition - vertexPosition);

	
	vec4 col = vec4(0.5,1.0, 0.5, 1.0);
	vec4 ambient = vec4(0.01,0.01,0.01,1.0);
	col = col * max(dot(normal, lightDirection), 0.0);

	float rim = 1.0 - dot(viewDirection, normal);

	gl_FragColor = ambient + col + pow(rim, 5.0) * vec4(0.1,0.5,1.0,1.0);

}
