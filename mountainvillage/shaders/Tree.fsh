#version 410 core

uniform int treePart;

out vec4 fragmentColor;

void main()
{
	if (treePart == 0)
		// trunk - brown (dark goldenrod)
		fragmentColor = vec4(0.722, 0.525, 0.043, 1.0);
	else // tree top - forest green
		fragmentColor = vec4(0.133, 0.545, 0.133, 1.0);
}
