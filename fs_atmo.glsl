$input v_pos, v_view, v_normal, v_color0


#include "./common/common.sh"

// math const
	const float PI = 3.14159265359;
const float DEG_TO_RAD = PI / 180.0;
const float MAX = 10000.0;

// scatter const
const float K_R = 0.166;
const float K_M = 0.0025;
const float E = 12.3; // light intensity
const vec3  C_R = vec3( 0.3, 0.7, 1.0 ); // 1 / wavelength ^ 4
const float G_M = -0.85;// Mie g

const float R = 1.0;
const float R_INNER = 0.94;
const float SCALE_H = 4.0 / ( R - R_INNER );
const float SCALE_L = 1.0 / ( R - R_INNER );

const int NUM_OUT_SCATTER = 20;
const float FNUM_OUT_SCATTER = 20.0;

const int NUM_IN_SCATTER = 20;
const float FNUM_IN_SCATTER = 20.0;

	
// ray direction
vec3 ray_dir( float fov, vec2 size, vec2 pos ) {
	vec2 xy = pos - size * 0.5;

	float cot_half_fov = tan( ( 90.0 - fov * 0.5 ) * DEG_TO_RAD );
	float z = size.y * 0.5 * cot_half_fov;

	return normalize( vec3( xy, -z ) );
}

float phase_mie( float g, float c, float cc ) {
	float gg = g * g;

	float a = ( 1.0 - gg ) * ( 1.0 + cc );

	float b = 1.0 + gg - 2.0 * g * c;
	b *= sqrt( b );
	b *= 2.0 + gg;

	return 1.5 * a / b;
}

vec2 ray_vs_sphere( vec3 p, vec3 dir, float r ) {
	float b = dot( p, dir );
	float c = dot( p, p ) - r * r;

	float d = b * b - c;
	if ( d < 0.0 ) {
		return vec2( MAX, -MAX );
	}
	d = sqrt( d );

	return vec2( -b - d, -b + d );
}

float phase_reyleigh(float cc)
{
	return 0.75*(1.0 + cc);
}


float density( vec3 p ){
	return exp( -( length( p ) - R_INNER ) * SCALE_H );
}

float optic( vec3 p, vec3 q ) {
	vec3 step = ( q - p ) / FNUM_OUT_SCATTER;


	vec3 v = p + step * 0.5;

	float sum = 0.0;
	for ( int i = 0; i < NUM_OUT_SCATTER; i++ ) {
		sum += density( v );
		v += step;
	}
	sum *= length( step ) * SCALE_L;

	return sum;
}

vec3 in_scatter( vec3 o, vec3 dir, vec2 e, vec3 l ) {
	float len = ( e.y - e.x ) / FNUM_IN_SCATTER;
	vec3 step = dir * len;
	vec3 p = o + dir * e.x;
	vec3 v = p + dir * ( len * 0.5 );

	vec3 sum = vec3( 0.0 );
	for ( int i = 0; i < NUM_IN_SCATTER; i++ ) {
		vec2 f = ray_vs_sphere( v, l, R );
		vec3 u = v + l * f.y;

		float n = ( optic( p, v ) + optic( v, u ) ) * ( PI * 4.0 );

		sum += density( v ) * exp( -n * ( K_R * C_R + K_M ) );

		v += step;
	}
	sum *= len * SCALE_L;

	float c  = dot( dir, -l );
	float cc = c * c;

	return sum * ( K_R * C_R * phase_reyleigh( cc ) + K_M * phase_mie( G_M, c, cc ) ) * E;
}

vec3 lightPosition = vec3(-5.0, 5.0, -5.0);
vec3 eye = vec3(0.0, 0.0, -4.0);
void main()
{
	vec2 iResolution = vec2(1280,800);
	vec3 dir = ray_dir( 45.0, iResolution.xy, gl_FragCoord.xy );

	// sun light dir
	vec3 l = normalize(v_pos - lightPosition);

	vec2 e = ray_vs_sphere( eye, dir, R );
	
	if ( e.x > e.y ) {
		discard;
	}	


	vec2 f = ray_vs_sphere( eye, dir, R_INNER );
	e.y = min( e.y, f.x );

	vec3 I = in_scatter( eye, dir, e, l );
	
	
	float avg = (I.x + I.y + I.z)/3;
	I += 0.4;
	gl_FragColor = vec4( I, avg );
	
}
