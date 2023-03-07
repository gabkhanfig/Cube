#pragma once

// Generated from Chunk.vert.
constexpr const char* generated_Chunk_vert = R"(#version 460 core

// https://sight.pages.ircad.fr/sight-doc/CodingStyle/src/07-glsl-style.html

layout (location = 0) in vec3 v_in_position;
layout (location = 1) in vec3 v_in_normal;
layout (location = 2) in vec2 v_in_texCoord;
layout (location = 3) in vec3 v_in_color;

uniform mat4 u_cameraMVP;
uniform vec3 u_chunkOffset;

// Interpolated fragment shader coordinates it's position in the triangle.
out vec3 v_out_fragCoord;
// Normalized coordinates of this vertex.
out flat vec3 v_out_vertCoord;
// Interpolated texture coordinates.
out vec2 v_out_texCoord;
// Interpolated color coordinates.
out vec3 v_out_color;

#define SUBVOXEL_COUNT 16.0

#define BYTE_1_BITMASK 0xFF
#define BYTE_2_BITMASK 0xFF00
#define BYTE_3_BITMASK 0xFF0000
#define BYTE_4_BITMASK 0xFF000000

void main()
{
	gl_Position = u_cameraMVP * vec4(v_in_position + u_chunkOffset, 1.0);

	v_out_vertCoord = v_in_position;
	v_out_fragCoord = v_in_position;
	v_out_texCoord = v_in_texCoord;
	v_out_color = v_in_color;
})";

// Generated from Chunk.frag.
constexpr const char* generated_Chunk_frag = R"(#version 460 core

// https://sight.pages.ircad.fr/sight-doc/CodingStyle/src/07-glsl-style.html

// Interpolated fragment shader coordinates it's position in the triangle.
in vec3 v_out_fragCoord;
// Normalized coordinates of this vertex.
in flat vec3 v_out_vertCoord;
// Interpolated texture coordinates.
in vec2 v_out_texCoord;
// Interpolated color coordinates.
in vec3 v_out_color;

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
	//const vec3 coord = floor(abs(_fragCoord * SUBVOXEL_COUNT - _vertCoord * SUBVOXEL_COUNT)) / SUBVOXEL_COUNT;
	//return coord;// + _vertCoord;
	return floor(_fragCoord * SUBVOXEL_COUNT) / SUBVOXEL_COUNT;
}

void main()
{
	CubicColors cols;
	cols.c000 = vec3(0, 0, 0); // non-z
	cols.c100 = vec3(1, 0, 0); // non-z
	cols.c010 = vec3(0, 1, 0); // non-z
	cols.c110 = vec3(1, 1, 0); // non-z
	cols.c001 = vec3(0, 0, 1);
	cols.c101 = vec3(1, 0, 1);
	cols.c011 = vec3(0, 1, 1);
	cols.c111 = vec3(1, 1, 1);

	//const vec3 subvoxel = GetRelativeSubvoxelPosition(v_out_fragCoord, v_out_vertCoord);
	//const vec3 outColor = TrilinearInterpolationColor(subvoxel, cols);
	const vec4 texColor = texture(u_Texture, v_out_texCoord);
	FragColor = texColor * vec4(v_out_color, 1);
	//FragColor = vec4(subvoxel, 1);
	//FragColor = texColor;
	//FragColor = vec4(vec3(outColor), 1);
})";

// Generated from InvalidTexture.png. Use sizeof operator to get the amount of bytes.
constexpr const unsigned char generated_InvalidTexture_png[192] = { 137, 80, 78, 71, 13, 10, 26, 10, 0, 0, 0, 13, 73, 72, 68, 82, 0, 0, 0, 16, 0, 0, 0, 16, 8, 6, 0, 0, 0, 31, 243, 255, 97, 0, 0, 0, 1, 115, 82, 71, 66, 0, 174, 206, 28, 233, 0, 0, 0, 122, 73, 68, 65, 84, 56, 79, 189, 211, 209, 9, 128, 48, 12, 4, 208, 187, 69, 156, 193, 81, 212, 65, 173, 163, 56, 131, 139, 68, 34, 68, 33, 26, 72, 9, 216, 159, 126, 148, 188, 54, 189, 150, 34, 34, 40, 12, 42, 64, 50, 36, 26, 214, 112, 109, 146, 25, 47, 96, 60, 128, 125, 120, 106, 186, 0, 45, 182, 97, 72, 26, 176, 157, 253, 156, 6, 108, 231, 82, 11, 138, 148, 1, 127, 229, 169, 22, 54, 182, 48, 170, 25, 75, 184, 166, 79, 232, 138, 49, 11, 248, 246, 186, 128, 175, 136, 211, 64, 20, 113, 26, 136, 34, 254, 31, 240, 113, 220, 39, 40, 252, 102, 156, 121, 220, 147, 209, 110, 202, 149, 43, 0, 0, 0, 0, 73, 69, 78, 68, 174, 66, 96, 130 };

// Generated from StoneBlock.png. Use sizeof operator to get the amount of bytes.
constexpr const unsigned char generated_StoneBlock_png[342] = { 137, 80, 78, 71, 13, 10, 26, 10, 0, 0, 0, 13, 73, 72, 68, 82, 0, 0, 0, 16, 0, 0, 0, 16, 8, 6, 0, 0, 0, 31, 243, 255, 97, 0, 0, 0, 1, 115, 82, 71, 66, 0, 174, 206, 28, 233, 0, 0, 1, 14, 73, 68, 65, 84, 56, 79, 117, 147, 177, 10, 133, 48, 12, 69, 219, 73, 116, 18, 220, 252, 64, 103, 87, 55, 191, 214, 77, 112, 170, 56, 249, 184, 129, 83, 98, 232, 235, 82, 109, 99, 114, 239, 73, 204, 251, 190, 191, 41, 165, 116, 93, 151, 182, 212, 247, 189, 237, 247, 125, 219, 62, 142, 99, 189, 227, 157, 248, 82, 74, 202, 235, 186, 90, 2, 45, 29, 12, 195, 96, 31, 29, 199, 97, 103, 211, 52, 89, 50, 221, 105, 233, 158, 165, 184, 188, 44, 139, 37, 152, 231, 217, 206, 159, 231, 105, 86, 71, 137, 20, 158, 231, 89, 11, 153, 2, 42, 179, 71, 233, 84, 167, 8, 234, 164, 38, 111, 219, 246, 34, 145, 0, 37, 80, 16, 118, 196, 7, 54, 93, 215, 85, 38, 149, 129, 175, 236, 89, 180, 158, 225, 192, 55, 102, 1, 95, 17, 18, 86, 224, 18, 45, 234, 62, 211, 70, 5, 9, 142, 108, 68, 249, 180, 151, 214, 214, 54, 40, 129, 186, 32, 175, 82, 209, 10, 160, 170, 103, 224, 91, 220, 156, 3, 47, 213, 247, 63, 14, 213, 103, 144, 116, 201, 132, 1, 79, 118, 152, 80, 63, 92, 116, 199, 24, 120, 136, 76, 89, 180, 19, 187, 4, 3, 197, 25, 68, 1, 212, 82, 143, 153, 1, 130, 188, 10, 159, 136, 34, 166, 32, 202, 255, 71, 189, 165, 228, 3, 17, 72, 30, 156, 255, 193, 226, 60, 212, 159, 73, 222, 21, 24, 161, 97, 3, 139, 122, 247, 99, 47, 192, 63, 161, 36, 3, 220, 225, 211, 159, 155, 0, 0, 0, 0, 73, 69, 78, 68, 174, 66, 96, 130, 0, 0 };
