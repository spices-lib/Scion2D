#version 450

in vec2 fragUVs;
in vec4 fragColor;
in float fragLineThickness;

out vec4 color;

void main()
{
	float distance = 1 - length(fragUVs);
	float wd = fwidth(distance);
	float circle = smoothstep(0, wd, distance);

	circle *= smoothstep(fragLineThickness + wd, fragLineThickness, distance);

	if (circle == 0.0)
		discard;

	color.a *= circle;
}