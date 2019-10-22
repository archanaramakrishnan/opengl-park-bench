#version 410 core

// phong.fsh - a fragment shader that implements a Phong Lighting model.

in PVA
{
	vec3 ecPosition;
	vec3 ecUnitNormal;
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
	vec3 liHat = vec3(0.0, 0.0, 1.0); // directional light in EC at eye (a flashlight)

	//I = ka*La + kd*(liHat.nHat) 
	//https://people.eecs.ku.edu/~jrmiller/Courses/672/InClass/3DLighting/3DLighting101.html
	return vec4((ka*La + (kd* dot(liHat, pvaIn.ecUnitNormal))), 1.0);
}

void main ()
{
	fragmentColor = evaluateLightingModel();
}
