$input v_pos, v_view, v_normal, v_color0


#include "./common/common.sh"

// math const
const float PI = 3.14159265359;
const float DEG_TO_RAD = PI / 180.0;
const float MAX = 10000.0;

// scatter const
const float K_R = 0.166;
const float K_M = 0.0025;
const float E = 14.3;
const vec3  C_R = vec3( 0.3, 0.7, 1.0 ); // 1 / wavelength ^ 4
const float G_M = -0.85;// Mie g
const float G_M2 = G_M*G_M;

const float R = 2.4;
const float R_INNER = 2.2;
const float SCALE_L = 1.0 / ( R - R_INNER ); //fScale
const float SCALE = 0.25;

const float SCALEDEPTH = 0.25;
const float SCALEOVERDEPTH = SCALE_L / SCALEDEPTH;

const int NUM_OUT_SCATTER = 20;
const float FNUM_OUT_SCATTER = 20.0;

const int NUM_IN_SCATTER = 20;
const float FNUM_IN_SCATTER = 20.0;


// ray direction
// took resolution ray generation from shadertoy.com
vec3 ray_dir( float fov, vec2 size, vec2 pos ) {
	vec2 xy = pos - size * 0.5;

	float cot_half_fov = tan( ( 90.0 - fov * 0.5 ) * DEG_TO_RAD );
	float z = size.y * 0.5 * cot_half_fov;

	return normalize( vec3( xy, -z ) );
}

// phase functions
//took this from the internet since writing the formulas out is kind of a pain
float getMiePhase(float fCos, float fCos2, float g, float g2)
{
	return 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + fCos2) / pow(abs(1.0 + g2 - 2.0*g*fCos), 1.5);
}

float getRayleighPhase(float fCos2)
{
	return 0.75 + 0.75*fCos2;
}

//r here is a magic number
//based off of arbitrary planet size since planets are blender meshes and do not have a defined R
vec2 rayIntersectSphere( vec3 p, vec3 dir, float r )
{
	float b = dot( p, dir );
	float c = dot( p, p ) - r * r;

	float d = b * b - c;
	if ( d < 0.0 ) {
		return vec2( MAX, -MAX );
	}
	d = sqrt( d );

	return vec2( -b - d, -b + d );
}


float scatteringCalc(vec3 v1, vec3 v2)
{
	//calculate the scattering within the atmosphere once our intersection hits the outer radius
	vec3 sampleRay = (v2 - v1) / FNUM_OUT_SCATTER;

	vec3 samplePoint = v1 + sampleRay * 0.5;

	float fScatter = 0.0;

	for(int i = 0; i < NUM_OUT_SCATTER; i++)
	{
		
		float height = length(samplePoint);
		float depth = exp(SCALEOVERDEPTH * (R_INNER - length(height)));
		fScatter += depth;
		samplePoint += sampleRay;
	}
	fScatter *= length(sampleRay) * SCALE_L;
	return fScatter;
}


vec3 lightPosition = vec3(-5.0, 6.0, 0.0);
uniform vec4 cameraPosition;
uniform vec4 resolution;


void main()
{
	vec3 eye = -cameraPosition.xyz;
	vec3 viewDirection = -normalize(v_view);
	vec2 res = resolution.xy;
	vec3 startingRay = ray_dir( 45.0, res.xy, gl_FragCoord.xy );
	// sun light dir
	vec3 lightDirection = normalize(v_pos - lightPosition);
	float cameraHeight = length(eye);

	float fFar = length(startingRay);

	vec2 intersection = rayIntersectSphere(eye, startingRay, R);
	if(intersection.x > intersection.y)
	{
		//ray did not hit sphere
		discard;
	}


	
	float sampleLength = (intersection.y - intersection.x) / FNUM_IN_SCATTER;
	vec3 sampleRay = startingRay * sampleLength;
	vec3 sampleRayStart = eye + startingRay * intersection.x;
	vec3 samplePoint = sampleRayStart + startingRay * (sampleLength * 0.5);

	float lightAngle = dot(startingRay, -lightDirection);
	float lightAngle2 = lightAngle * lightAngle;

	vec3 finalColor = vec3(0.0, 0.0, 0.0);

	for(int i = 0; i < NUM_IN_SCATTER; i++)
	{
		float height = length(samplePoint);
		float depth =  exp(SCALEOVERDEPTH * (R_INNER - length(height)));

		//float lightAngle = dot(lightDirection, samplePoint) / height;
		//float cameraAngle = dot(startingRay, samplePoint) / height;
		vec2 sampleIntersection = rayIntersectSphere(samplePoint, lightDirection, R);
		vec3 sampleLightRay = samplePoint + lightDirection * sampleIntersection.y;
		//this is an attenuation calculation
		//took the double scattering calcs from shadertoy as well
		//GPUGems method was faulty with this code for some reason
		float n = ( scatteringCalc( sampleRayStart, samplePoint ) + scatteringCalc( samplePoint, sampleLightRay ) ) * ( PI * 4.0 );

		
		finalColor += depth * exp( -n * ( K_R * C_R + K_M ) );
		samplePoint += sampleRay;
	}
	finalColor *= sampleLength * SCALE_L;
	finalColor = finalColor * ( K_R * C_R * getRayleighPhase( lightAngle2 ) + K_M * getMiePhase( lightAngle, lightAngle2, G_M, G_M2 ) ) * E;
	float avg = (finalColor.x + finalColor.y + finalColor.z) /3;
	gl_FragColor = vec4( finalColor, avg );
	
}
