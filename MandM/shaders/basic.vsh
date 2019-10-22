#version 410 core

// basic.vsh

// Naming convention for variables holding coordinates:
// mc - model coordinates
// ec - eye coordinates
// lds - logical device space
// "p_" prefix on any of the preceding indicates the coordinates have been
//      embedded in projective space
// (gl_Position would be called something like: p_ldsPosition)

// Transformation Matrices
uniform mat4 mc_ec =   // (dynamic rotations) * (ViewOrientation(E,C,up))
	mat4(1.0, 0.0, 0.0, 0.0, // initialize to identity matrix
	     0.0, 1.0, 0.0, 0.0, // ==> no dynamic rotations -AND- MC = EC
	     0.0, 0.0, 1.0, 0.0,
	     0.0, 0.0, 0.0, 1.0);
uniform mat4 ec_lds = // (W-V map) * (projection matrix)
	mat4(1.0, 0.0, 0.0, 0.0, // initialize to (almost) identity matrix
	     0.0, 1.0, 0.0, 0.0, // ==> ORTHOGONAL projection -AND- EC = LDS
	     0.0, 0.0, -1.0, 0.0,
	     0.0, 0.0, 0.0, 1.0);

// Per-vertex attributes
// 1. incoming vertex position in model coordinates
layout (location = 0) in vec3 mcPosition;
// 2. incoming vertex normal vector in model coordinates
in vec3 mcNormal; // incoming normal vector in model coordinates

// The lighting model will be computed in the fragment shader, so we
// just need to pass on the per-vertex information it needs to do so.
// The lighting model is computed in eye coordinate space, hence:
out PVA
{
	vec3 ecPosition;
	vec3 ecUnitNormal;
} pvaOut;

void main ()
{
	// convert current vertex and its associated normal to eye coordinates
	vec4 p_ecPosition = mc_ec * vec4(mcPosition, 1.0);
	pvaOut.ecPosition = p_ecPosition.xyz/p_ecPosition.w;
	mat3 normalMatrix = transpose( inverse( mat3x3(mc_ec) ) );
	pvaOut.ecUnitNormal = normalize(normalMatrix * mcNormal);

	// OpenGL expects us to set "gl_Position" to the projective space
	// representation of the 3D logical device space coordinates of the
	// input vertex:
	gl_Position = ec_lds * p_ecPosition;
}
