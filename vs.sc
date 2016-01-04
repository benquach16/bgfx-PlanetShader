$input a_position, a_color0, a_normal
$output v_color0, v_view0, v_constant

/*
 * Copyright 2011-2015 Branimir Karadzic. All rights reserved.
 * License: http://www.opensource.org/licenses/BSD-2-Clause
 */

#include "./common/common.sh"

void main()
{
	vec4 finalPos = mul(u_modelViewProj, vec4(a_position, 1.0) );
	vec3 position = finalPos.xyz;
	
	//normalize?
	//destroyes scale
	//finalPos = normalize(finalPos);
	//finalPos.w = 10.0;

	gl_Position = finalPos;

	vec3 view = mul(u_view, vec4(a_position, 1.0) ).xyz;
	v_color0 = a_color0;
	v_view0 = view;
	vec3 direction = normalize(position - view);

	v_constant = pow(1.0 + dot(direction, a_normal), 5);
}	
