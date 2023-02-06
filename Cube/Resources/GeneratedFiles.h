#pragma once

// Generated from Chunk.vert.
constexpr const char* generated_Chunk_vert = R"(#version 460 core

layout (location = 0) in vec3 position;
layout	(location = 1) in vec2 texCoord;

uniform mat4 u_cameraMVP;

// Interpolated fragment shader coordinates it's position in the triangle
out vec3 fragCoord;
// Normalized coordinates of this vertex
out flat vec3 vertCoord;
//
out vec2 v_TexCoord;

#define SUBVOXEL_COUNT 16.0

#define BYTE_1_BITMASK 0xFF
#define BYTE_2_BITMASK 0xFF00
#define BYTE_3_BITMASK 0xFF0000
#define BYTE_4_BITMASK 0xFF000000

void main()
{
	//const vec3 position = PositionFromBitmask(aPos) / 255.0 - 0.5;
	gl_Position = u_cameraMVP * vec4(position, 1.0);

	vertCoord = position;
	fragCoord = position;
	v_TexCoord = texCoord;
})";

// Generated from Chunk.frag.
constexpr const char* generated_Chunk_frag = R"(#version 460 core

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
	vec3 c010;
	vec3 c110;
	vec3 c001;
	vec3 c101;
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
	cols.c100 = vec3(0.5, 0.4, 0.7); // non-z
	cols.c010 = vec3(0.2, 0.2, 0.9); // non-z
	cols.c110 = vec3(0.1, 0.8, 0.1); // non-z
	cols.c001 = vec3(1, 1, 1);
	cols.c101 = vec3(0, 0, 0);
	cols.c011 = vec3(1, 1, 1);
	cols.c111 = vec3(1, 1, 1);

	const vec3 subvoxel = GetSubvoxelPosition(fragCoord, vertCoord);
	const vec3 outColor = TrilinearInterpolationColor(subvoxel, cols);
	const vec4 texColor = texture(u_Texture, v_TexCoord);
	FragColor = texColor * vec4(outColor, 1);
})";

// Generated from 16x16wood.png. Use sizeof operator to get the amount of bytes.
constexpr const unsigned char generated_16x16wood_png[139] = { 137, 80, 78, 71, 13, 10, 26, 10, 0, 0, 0, 13, 73, 72, 68, 82, 0, 0, 0, 16, 0, 0, 0, 16, 8, 6, 0, 0, 0, 31, 243, 255, 97, 0, 0, 0, 1, 115, 82, 71, 66, 0, 174, 206, 28, 233, 0, 0, 0, 69, 73, 68, 65, 84, 56, 79, 99, 156, 19, 101, 243, 159, 129, 2, 192, 8, 50, 32, 178, 43, 1, 110, 196, 242, 178, 5, 12, 200, 124, 108, 102, 195, 212, 128, 104, 234, 24, 64, 129, 15, 48, 93, 0, 51, 12, 155, 87, 208, 197, 176, 122, 129, 44, 3, 168, 234, 133, 209, 88, 64, 4, 39, 73, 209, 56, 194, 99, 1, 0, 104, 64, 125, 33, 239, 7, 129, 76, 0, 0, 0, 0, 73, 69, 78, 68, 174, 66, 96, 130 };

// Generated from 16x16black.png. Use sizeof operator to get the amount of bytes.
constexpr const unsigned char generated_16x16black_png[139] = { 137, 80, 78, 71, 13, 10, 26, 10, 0, 0, 0, 13, 73, 72, 68, 82, 0, 0, 0, 16, 0, 0, 0, 16, 8, 6, 0, 0, 0, 31, 243, 255, 97, 0, 0, 0, 1, 115, 82, 71, 66, 0, 174, 206, 28, 233, 0, 0, 0, 69, 73, 68, 65, 84, 56, 79, 99, 100, 96, 96, 248, 207, 64, 1, 96, 4, 25, 240, 245, 201, 28, 184, 17, 220, 50, 41, 12, 200, 124, 108, 102, 195, 212, 128, 104, 234, 24, 64, 129, 15, 48, 93, 0, 51, 12, 155, 87, 208, 197, 176, 122, 129, 44, 3, 168, 234, 133, 209, 88, 64, 4, 39, 73, 209, 56, 194, 99, 1, 0, 125, 103, 106, 1, 114, 191, 206, 5, 0, 0, 0, 0, 73, 69, 78, 68, 174, 66, 96, 130 };

// Generated from 16x16blue.png. Use sizeof operator to get the amount of bytes.
constexpr const unsigned char generated_16x16blue_png[139] = { 137, 80, 78, 71, 13, 10, 26, 10, 0, 0, 0, 13, 73, 72, 68, 82, 0, 0, 0, 16, 0, 0, 0, 16, 8, 6, 0, 0, 0, 31, 243, 255, 97, 0, 0, 0, 1, 115, 82, 71, 66, 0, 174, 206, 28, 233, 0, 0, 0, 69, 73, 68, 65, 84, 56, 79, 99, 100, 216, 254, 254, 63, 3, 5, 128, 17, 100, 192, 87, 221, 181, 112, 35, 184, 47, 7, 51, 32, 243, 177, 153, 13, 83, 3, 162, 169, 99, 0, 5, 62, 192, 116, 1, 204, 48, 108, 94, 65, 23, 195, 234, 5, 178, 12, 160, 170, 23, 70, 99, 1, 17, 156, 36, 69, 227, 8, 143, 5, 0, 241, 224, 132, 97, 201, 86, 203, 68, 0, 0, 0, 0, 73, 69, 78, 68, 174, 66, 96, 130 };
