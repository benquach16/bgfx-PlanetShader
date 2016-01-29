$input v_texcoord0

/*
 * Copyright 2011-2016 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include "./common/common.sh"

SAMPLERCUBE(skybox, 1);

uniform mat4 u_mtx;

void main()
{
	gl_FragColor = vec4(0.5, 0.7, 0.5, 1.0);
}
