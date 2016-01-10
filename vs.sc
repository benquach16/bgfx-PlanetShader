$input a_position, a_normal
$output v_pos, v_view, v_normal, v_color0

#include "./common/common.sh"

void main()
{
	vec3 normal = a_normal.xyz;
	vec3 position = a_position;
	gl_Position = mul(u_modelViewProj, vec4(position, 1.0) );
	v_pos = gl_Position.xyz;
	v_view = mul(u_modelView, vec4(position, 1.0) ).xyz;
	v_normal = mul(u_modelView, vec4(normal, 0.0) ).xyz;
	v_color0 = vec4(1.0, 1.0, 1.0, 1.0);

}
