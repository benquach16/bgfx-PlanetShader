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
	vec3 viewPosition = normalize(v_pos);
	vec3 lightPosition = vec3(0.0, 0.0, -5.0);
	vec3 lightDirection = normalize(lightPosition - viewPosition);
	vec3 R = normalize(-reflect(lightDirection, normal));

	
	vec4 col = vec4(0.5,1.0, 0.5, 1.0);
	vec4 ambient = vec4(0.01,0.01,0.01,1.0);
	col = col * max(dot(normal, lightDirection), 0.0);

	
	col = clamp(col, 0.0, 1.0);

	float dotProduct = dot(viewPosition, normal);
	float fresnel = fresnel(dotProduct, 0.2, 9.0);
	vec4 fresCol = fresnel*vec4(0.01,0.02,0.1, 1.0);
	gl_FragColor = ambient + col + fresCol;

}
