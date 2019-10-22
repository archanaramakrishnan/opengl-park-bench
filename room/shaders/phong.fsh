#version 410 core

// phong.fsh - a fragment shader that implements a Phong Lighting model
//             and an optional texture.

in PVA
{
	vec3 ecPosition;
	vec3 ecUnitNormal;
	vec2 texCoords;
} pvaIn;

// For lighing model:
uniform mat4 ec_lds; // so projection type and hence vHat can be determined

// Phong material properties (RGB reflectances);
uniform vec3 ka = vec3(0.8, 0.0, 0.0); // default: darkish red
uniform vec3 kd = vec3(0.8, 0.0, 0.0); // default: darkish red
// Lighting environment
// RGB strength of assumed ambient light:
uniform vec3 La = vec3(0.15, 0.15, 0.15);

// output color from the lighting model:
out vec4 fragmentColor;

vec4 evaluateLightingModel()
{
	// THIS IS JUST A PLACEHOLDER FOR A LIGHTING MODEL.
	// It only currently implements simple Lambert shading.

	// NOTE: We assume a single directional light source defined in EC (liHat).
	//
	// In project 3, several aspects of this will be generalized.

	vec3 liHat = vec3(0.0, 0.0, 1.0); // directional light in EC at eye (a flashlight)

	// Use liHat, the uniforms, and the incoming PVA values to compute
	// the simplified Phong model we are using for project 2.

	// For now:
	return vec4(0.0, 0.0, 0.0, 1.0);
}

vec4 evaluateTexture()
{
	// THIS IS JUST A PLACEHOLDER FOR TEXTURE MAPPING CODE.
	// This meaningless code is here only so that "texCoords" will be found by
	// CPU side lookup code. Delete these comments and ALL THREE of the following
	// lines of code before developing a "real" implementation.
	if (pvaIn.texCoords.s >= pvaIn.texCoords.t)
		return vec4(0.0);
	return vec4(0.001);
}

void main ()
{
	vec4 phongColor = evaluateLightingModel();
	vec4 textureColor = evaluateTexture();
	// How should the two be combined? Here's one common way to simulate the
	// lighting model affecting a polygon along with its single texture:
	fragmentColor = phongColor * textureColor;
}
