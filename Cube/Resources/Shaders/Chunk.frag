#version 460 core

// https://sight.pages.ircad.fr/sight-doc/CodingStyle/src/07-glsl-style.html

// Interpolated fragment shader coordinates it's position in the triangle.
in vec3 v_out_fragCoord;
// Normalized coordinates of this vertex.
in flat vec3 v_out_vertCoord;
// Interpolated texture coordinates.
in vec2 v_out_texCoord;

uniform sampler2D u_Texture;

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
	vec3 c010;
	vec3 c110;
	vec3 c001;
	vec3 c101;
	vec3 c011;
	vec3 c111;
};

/* 0, 0, 0 top back right. 1, 1, 1 bottom front left 
https://en.wikipedia.org/wiki/Trilinear_interpolation */ 
vec3 TrilinearInterpolationColor(const vec3 _coord, const CubicColors _col) 
{
	// Remove integer part example: 1.5 -> 0.5
	const vec3 coord = _coord;//vec3(_coord - floor(_coord));

	const vec3 c00 = _col.c000 * (1 - coord.x) + _col.c100 * coord.x;
	const vec3 c01 = _col.c001 * (1 - coord.x) + _col.c101 * coord.x;
	const vec3 c10 = _col.c010 * (1 - coord.x) + _col.c110 * coord.x;
	const vec3 c11 = _col.c011 * (1 - coord.x) + _col.c111 * coord.x;

	const vec3 c0 = c00 * (1 - coord.y) + c10 * coord.y;
	const vec3 c1 = c01 * (1 - coord.y) + c11 * coord.y;

	const vec3 c = c0 * (1 - coord.z) + c1 * coord.z;

	return c;
}

/* Get the relative subvoxel position. Clamped to the 16x16 full block area. */
vec3 GetRelativeSubvoxelPosition(const vec3 _fragCoord, const vec3 _vertCoord) 
{
	const vec3 coord = floor(abs(_fragCoord * SUBVOXEL_COUNT - _vertCoord * SUBVOXEL_COUNT)) / SUBVOXEL_COUNT;
	return coord;// + _vertCoord;
}

void main()
{
	CubicColors cols;
	cols.c000 = vec3(1, 1, 1); // non-z
	cols.c100 = vec3(1, 0, 0); // non-z
	cols.c010 = vec3(0, 1, 0); // non-z
	cols.c110 = vec3(0, 0, 1); // non-z
	cols.c001 = vec3(1, 1, 1);
	cols.c101 = vec3(0, 0, 0);
	cols.c011 = vec3(1, 1, 1);
	cols.c111 = vec3(1, 1, 1);

	const vec3 subvoxel = GetRelativeSubvoxelPosition(v_out_fragCoord, v_out_vertCoord);
	const vec3 outColor = TrilinearInterpolationColor(subvoxel, cols);
	const vec4 texColor = texture(u_Texture, v_out_texCoord);
	//FragColor = texColor * vec4(outColor, 1);
	FragColor = vec4(subvoxel, 1);

	//FragColor = vec4(vec3(outColor), 1);
}