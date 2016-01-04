$input v_color0, v_view0, v_constant

/*
 * Copyright 2011-2015 Branimir Karadzic. All rights reserved.
 * License: http://www.opensource.org/licenses/BSD-2-Clause
 */

#include "./common/common.sh"

void main()
{
	//create light vector here
	vec3 lightPos = vec3(0.0,0.0,0.0);
	vec4 col = vec4(1.0,1.0,1.0,1.0);
	vec4 col2 = vec4(0.0, 0.0, 0.0, 0.0);
	gl_FragColor = mix(col, col2, v_constant);
	
	
}
