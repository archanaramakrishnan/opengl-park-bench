#version 410 core

in float heightFractionToFS;
out vec4 fragmentColor;

// for better control, these should be passed as uniforms:
uniform vec4 minRGB = vec4(0.0, 0.0, 0.0, 1.0);
uniform vec4 midRGB = vec4(0.3, 0.4, 0.3, 1.0);
uniform vec4 maxRGB = vec4(1.0, 1.0, 1.0, 1.0);

void main()
{
	// background tall mountain
	float midThreshhold = 0.65;
	if (heightFractionToFS <= midThreshhold)
		fragmentColor = mix(minRGB, midRGB, heightFractionToFS/midThreshhold);
	else
	{
		float f = (heightFractionToFS - midThreshhold) / (1.0 - midThreshhold);
		fragmentColor = mix(midRGB, maxRGB, f);
	}
}
