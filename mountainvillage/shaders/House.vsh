#version 410 core

layout (location = 0) in vec2 mcPosition;
in vec2 texCoords;

uniform vec4 scaleTrans;

out vec2 texCoordsToFS;

void main()
{
	texCoordsToFS = texCoords;
	float ldsX = scaleTrans[0]*mcPosition.x + scaleTrans[1];
	float ldsY = scaleTrans[2]*mcPosition.y + scaleTrans[3];
	gl_Position = vec4(ldsX, ldsY, 0, 1);
}
