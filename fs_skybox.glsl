$input v_texcoord0

/*
 * Copyright 2011-2016 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include "./common/common.sh"

SAMPLER2D(tex, 0);

uniform mat4 u_mtx;

void main()
{
    gl_FragColor = toLinearAccurate(texture2D(tex, v_texcoord0.xy));
	//gl_FragColor = vec4(0.5, 1.0, 0.3, 1.0);
}
