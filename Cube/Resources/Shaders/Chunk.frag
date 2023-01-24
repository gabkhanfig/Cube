#version 460 core

// Interpolated fragment shader coordinates it's position in the triangle
in vec3 fragCoord;
// Normalized coordinates of this vertex
in flat vec3 vertCoord;
//
in vec2 v_TexCoord;

uniform sampler2D u_Texture;
uniform vec4 hello;

out vec4 FragColor;

#define SUBVOXEL_COUNT 16.0

#define BYTE_1_BITMASK 0xFF
#define BYTE_2_BITMASK 0xFF00
#define BYTE_3_BITMASK 0xFF0000
#define BYTE_4_BITMASK 0xFF000000


layout(std430, binding = 4) buffer TestBuffer
{
	vec4 ssbocol;
};

/*  */
struct CubicColors
{
	vec3 c000;
	vec3 c100;
	vec3 c001;
	vec3 c101;
	vec3 c010;
	vec3 c110;
	vec3 c011;
	vec3 c111;
};

/* 0, 0, 0 top back right. 1, 1, 1 bottom front left 
https://en.wikipedia.org/wiki/Trilinear_interpolation */ 
vec3 TrilinearInterpolationColor(const vec3 coord, const CubicColors col) 
{
	const vec3 c00 = col.c000 * (1 - coord.x) + col.c100 * coord.x;
	const vec3 c01 = col.c001 * (1 - coord.x) + col.c101 * coord.x;
	const vec3 c10 = col.c010 * (1 - coord.x) + col.c110 * coord.x;
	const vec3 c11 = col.c011 * (1 - coord.x) + col.c111 * coord.x;

	const vec3 c0 = c00 * (1 - coord.y) + c10 * coord.y;
	const vec3 c1 = c01 * (1 - coord.y) + c11 * coord.y;

	const vec3 c = c0 * (1 - coord.z) + c1 * coord.z;

	return c;
}

/* Get the subvoxel position. Clamped to the 16x16 full block area. */
vec3 GetSubvoxelPosition(const vec3 _fragCoord, const vec3 _vertCoord) 
{
	const vec3 coord = floor(abs(_fragCoord * SUBVOXEL_COUNT + _vertCoord * SUBVOXEL_COUNT)) / SUBVOXEL_COUNT;
	return coord;
}

/* Interpolate the 4 colors given the fragment's 2d coordinate on the quad. Will set the colour per subvoxel. */ 
vec3 InterpolateColor(vec3 topRight, vec3 topLeft, vec3 bottomRight, vec3 bottomLeft) 
{
	// Clamp the coordinates to 0-15 and then normalize to 0-1.
	const vec2 coord = vec2(floor(abs(fragCoord * SUBVOXEL_COUNT - vertCoord * SUBVOXEL_COUNT)) / SUBVOXEL_COUNT);

	// Clamped bilinear interpolation
	const vec3 Color = vec3(
		(1.0 - coord.x) * (1.0 - coord.y) * topRight +
		coord.x * (1.0 - coord.y) * topLeft +
		(1.0 - coord.x) * coord.y * bottomRight +
		coord.x * coord.y * bottomLeft
	);

	return vec3(Color);
}

void main()
{
	/*
	With this ibo:
	indices[6] = {
		0, 1, 2,
		3, 0, 2
	};
	Top right is 0, 0
	Bottom left is 1, 1
	*/

	CubicColors cols;
	cols.c000 = vec3(1, 1, 1); // non-z
	cols.c100 = vec3(0.1, 0.8, 0.5); // non-z
	cols.c001 = vec3(1, 1, 0.1);
	cols.c101 = vec3(0, 0, 0);
	cols.c010 = vec3(1, 0.2, 0.5); // non-z
	cols.c110 = vec3(1, 0.5, 0.2); // non-z
	cols.c011 = vec3(1, 1, 1);
	cols.c111 = vec3(1, 1, 1);

	const vec3 subvoxel = GetSubvoxelPosition(fragCoord, vertCoord);
	const vec3 outColor = TrilinearInterpolationColor(subvoxel, cols);
	const vec4 texColor = texture(u_Texture, v_TexCoord);
	FragColor = texColor * vec4(outColor, 1);
	//FragColor = texColor;
	//FragColor = vec4(outColor, 1);
	//FragColor = ssbocol;
}