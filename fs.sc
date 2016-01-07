$input v_color0, v_view0, v_constant

/*
 * Copyright 2011-2015 Branimir Karadzic. All rights reserved.
 * License: http://www.opensource.org/licenses/BSD-2-Clause
 */

#include "./common/common.sh"


// scatter const
const float K_R = 0.166;
const float K_M = 0.0025;
const float E = 14.3; // light intensity
const vec3  C_R = vec3( 0.3, 0.7, 1.0 ); // 1 / wavelength ^ 4
const float G_M = -0.85;// Mie g

const float R = 1.0;
const float R_INNER = 0.7;
const float SCALE_H = 4.0 / ( R - R_INNER );
const float SCALE_L = 1.0 / ( R - R_INNER );

const int NUM_OUT_SCATTER = 10;
const float FNUM_OUT_SCATTER = 10.0;

const int NUM_IN_SCATTER = 10;
const float FNUM_IN_SCATTER = 10.0;

float phase_reyleigh( float cc )
{
	return 0.75 * ( 1.0 + cc );
}

float density( vec3 p )
{
	return exp( -( length( p ) - R_INNER ) * SCALE_H );
}



void main()
{
	//create light vector here
	vec3 lightPos = vec3(0.0,0.0,0.0);
	vec4 col = vec4(1.0,1.0,1.0,1.0);
	vec4 col2 = vec4(0.0, 0.0, 0.0, 0.0);

  	vec3 sunlight = vec3(10.0, 0.0, 0.0);
	gl_FragColor = mix(col2, col, v_constant);
	
}
