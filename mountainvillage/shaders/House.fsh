#version 410 core

in vec2 texCoordsToFS;

uniform int housePart;
uniform int renderingFontString;
uniform sampler2D fontTextureMap;
uniform vec4 fontColor;

out vec4 fragmentColor;

void main()
{
	if (renderingFontString == 1)
		fragmentColor = texture(fontTextureMap, texCoordsToFS) * fontColor;
	else if (housePart == 0) // Roof - gray
		fragmentColor = vec4(0.2, 0.2, 0.2, 1.0);
	else if (housePart == 1) // walls - Firebrick
		fragmentColor = vec4(0.698, 0.133, 0.133, 1.0);
	else // door - light goldenrod
		fragmentColor = vec4(0.933, 0.982, 0.510, 1.0);
}
