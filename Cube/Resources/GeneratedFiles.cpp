const char* generated_Chunk_vert = R"(#version 460 core

// https://sight.pages.ircad.fr/sight-doc/CodingStyle/src/07-glsl-style.html

layout (location = 0) in vec3 v_in_position;
layout (location = 1) in uint v_in_packedNormal;
layout (location = 2) in vec2 v_in_texCoord;
layout (location = 3) in uint v_in_packedColor;

// Camera Model-View-Projection matrix.
uniform mat4 u_cameraMVP;
// Position offset of the chunk that is being drawn.
uniform vec3 u_chunkOffset;

// Interpolated fragment shader coordinates it's position in the triangle.
//out vec3 v_out_fragCoord;
// Normalized coordinates of this vertex.
//out flat vec3 v_out_vertCoord;
// Interpolated texture coordinates.
out vec2 v_out_texCoord;
// Interpolated color coordinates.
out vec4 v_out_color;
// Color scale value depending on quad normal
out float v_out_normalColorScale;

#define SUBVOXEL_COUNT 16.0

#define BYTE_1_BITMASK 0xFF
#define BYTE_2_BITMASK 0xFF00
#define BYTE_3_BITMASK 0xFF0000
#define BYTE_4_BITMASK 0xFF000000

float ScaleColorByNormal(vec3 normal) 
{
#define X_SCALE(x) ((1 - abs(normal.y)) * ((1 - abs(x)) * 0.8))
#define Y_SCALE(y) (((y + 1.f) / 8) + 0.75)
#define Z_SCALE(z) ((1 - abs(normal.y)) * ((1 - abs(z)) * 0.9))

	return max(max(X_SCALE(normal.x), Y_SCALE(normal.y)), Z_SCALE(normal.x));
}

vec4 UnpackColor(uint packedColor) {
	return vec4(
		float(packedColor & 255U) / 255.f,
		float(packedColor >> 8 & 255U) / 255.f,
		float(packedColor >> 16 & 255U) / 255.f,
		float(packedColor >> 24 & 255U) / 255.f
	);
}

vec3 UnpackNormal(uint packedNormal) {
	return vec3(
		(float(packedNormal & 255U) - 127.f) / 127.f,
		(float(packedNormal >> 8 & 255U) - 127.f) / 127.f,
		(float(packedNormal >> 16 & 255U) - 127.f) / 127.f
	);
}

void main()
{
	gl_Position = u_cameraMVP * vec4(v_in_position + u_chunkOffset, 1.0);

	v_out_texCoord = v_in_texCoord;
	v_out_color = UnpackColor(v_in_packedColor);

	v_out_normalColorScale = ScaleColorByNormal(UnpackNormal(v_in_packedNormal));
})";

const char* generated_Chunk_frag = R"(#version 460 core

// https://sight.pages.ircad.fr/sight-doc/CodingStyle/src/07-glsl-style.html

// Interpolated fragment shader coordinates it's position in the triangle.
//in vec3 v_out_fragCoord;
// Normalized coordinates of this vertex.
//in flat vec3 v_out_vertCoord;
// Interpolated texture coordinates.
in vec2 v_out_texCoord;
// Interpolated color coordinates.
in vec4 v_out_color;
// Color scale value depending on quad normal
in float v_out_normalColorScale;

uniform sampler2D u_Texture;

out vec4 FragColor;

#define SUBVOXEL_COUNT 16.0

#define BYTE_1_BITMASK 0xFF
#define BYTE_2_BITMASK 0xFF00
#define BYTE_3_BITMASK 0xFF0000
#define BYTE_4_BITMASK 0xFF000000

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

vec3 unpackColor(uint packedColor) {
	return vec3(
		float(packedColor & 255) / 255.f,
		float(packedColor >> 8 & 255) / 255.f,
		float(packedColor >> 16 & 255) / 255.f
	);
}

void main()
{
	const vec4 texColor = texture(u_Texture, v_out_texCoord);
	FragColor = texColor * v_out_color * v_out_normalColorScale;
})";

const char* generated_Block_vert = R"(#version 460 core

// https://sight.pages.ircad.fr/sight-doc/CodingStyle/src/07-glsl-style.html

layout (location = 0) in uint v_in_packedOffsetPosition; // PackedBlockOffsetPosition struct
layout (location = 1) in uint v_in_packedNormal; // PackedNormal struct
layout (location = 2) in vec2 v_in_texCoord; // glm::vec2
layout (location = 3) in uint v_in_packedColor; // PackedColor struct

// Camera Model-View-Projection matrix.
uniform mat4 u_cameraMVP;
// Position offset of the chunk that is being drawn.
uniform vec3 u_chunkOffset;

// Interpolated fragment shader coordinates it's position in the triangle.
//out vec3 v_out_fragCoord;
// Normalized coordinates of this vertex.
//out flat vec3 v_out_vertCoord;
// Interpolated texture coordinates.
out vec2 v_out_texCoord;
// Interpolated color coordinates.
out vec4 v_out_color;
// Color scale value depending on quad normal
out float v_out_normalColorScale;

#define SUBVOXEL_COUNT 16.0

#define BYTE_1_BITMASK 0xFF
#define BYTE_2_BITMASK 0xFF00
#define BYTE_3_BITMASK 0xFF0000
#define BYTE_4_BITMASK 0xFF000000
#define FIRST_10_BITS 1023

#define CHUNK_LENGTH 8

float ScaleColorByNormal(vec3 normal) 
{
#define X_SCALE(x) ((1 - abs(normal.y)) * ((1 - abs(x)) * 0.8))
#define Y_SCALE(y) (((y + 1.f) / 8) + 0.75)
#define Z_SCALE(z) ((1 - abs(normal.y)) * ((1 - abs(z)) * 0.9))

	return max(max(X_SCALE(normal.x), Y_SCALE(normal.y)), Z_SCALE(normal.x));
}

vec4 UnpackColor(uint packedColor) {
	return vec4(
		float(packedColor & 255U) / 255.f,
		float(packedColor >> 8 & 255U) / 255.f,
		float(packedColor >> 16 & 255U) / 255.f,
		float(packedColor >> 24 & 255U) / 255.f
	);
}

vec3 UnpackNormal(uint packedNormal) {
	return vec3(
		(float(packedNormal & FIRST_10_BITS) - 511.f) / 511.f,
		(float((packedNormal >> 10) & FIRST_10_BITS) - 511.f) / 511.f,
		(float((packedNormal >> 20) & FIRST_10_BITS) - 511.f) / 511.f
	);
}


float UnpackOffsetPositionComponent(uint bitmask) {
	const float negativeShift = float((bitmask >> 8) & 1) * CHUNK_LENGTH * -1; // 9th bit is negative offset
	const float positiveShift = float((bitmask >> 9) & 1) * CHUNK_LENGTH; // 10th bit is positive offset
	const float withinChunkBound = float(bitmask & 255) / 16.f;
	return withinChunkBound + negativeShift + positiveShift;
}

vec3 UnpackOffsetPosition(uint offsetPosition) {
	const uint xBitmask = offsetPosition & FIRST_10_BITS;
	const uint yBitmask = (offsetPosition >> 10) & FIRST_10_BITS;
	const uint zBitmask = (offsetPosition >> 20) & FIRST_10_BITS;
	return vec3(
		UnpackOffsetPositionComponent(xBitmask),
		UnpackOffsetPositionComponent(yBitmask),
		UnpackOffsetPositionComponent(zBitmask)
	);
}

void main()
{
	const vec3 vertexPosition = UnpackOffsetPosition(v_in_packedOffsetPosition);
	gl_Position = u_cameraMVP * vec4(vertexPosition + u_chunkOffset, 1.0);

	v_out_texCoord = v_in_texCoord;
	v_out_color = UnpackColor(v_in_packedColor);

	v_out_normalColorScale = ScaleColorByNormal(UnpackNormal(v_in_packedNormal));
})";

const char* generated_Block_frag = R"(#version 460 core

// https://sight.pages.ircad.fr/sight-doc/CodingStyle/src/07-glsl-style.html

// Interpolated fragment shader coordinates it's position in the triangle.
//in vec3 v_out_fragCoord;
// Normalized coordinates of this vertex.
//in flat vec3 v_out_vertCoord;
// Interpolated texture coordinates.
in vec2 v_out_texCoord;
// Interpolated color coordinates.
in vec4 v_out_color;
// Color scale value depending on quad normal
in float v_out_normalColorScale;

uniform sampler2D u_Texture;

out vec4 FragColor;

#define SUBVOXEL_COUNT 16.0

#define BYTE_1_BITMASK 0xFF
#define BYTE_2_BITMASK 0xFF00
#define BYTE_3_BITMASK 0xFF0000
#define BYTE_4_BITMASK 0xFF000000

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

vec3 unpackColor(uint packedColor) {
	return vec3(
		float(packedColor & 255) / 255.f,
		float(packedColor >> 8 & 255) / 255.f,
		float(packedColor >> 16 & 255) / 255.f
	);
}

void main()
{
	const vec4 texColor = texture(u_Texture, v_out_texCoord);
	FragColor = texColor * v_out_color * v_out_normalColorScale;
})";

unsigned char generated_InvalidTexture_png[1024] = { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 48, 226, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 48, 226, 0, 255, 48, 226, 0, 255, 48, 226, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 48, 226, 0, 255, 0, 0, 0, 255, 48, 226, 0, 255, 0, 0, 0, 255, 48, 226, 0, 255, 0, 0, 0, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 48, 226, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 48, 226, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 255, 255, 255, 255, 255, 255, 255, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 48, 226, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 0, 0, 0, 255, 0, 0, 0, 255, 48, 226, 0, 255, 48, 226, 0, 255, 48, 226, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 0, 0, 0, 255, 48, 226, 0, 255, 0, 0, 0, 255, 48, 226, 0, 255, 0, 0, 0, 255, 48, 226, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 48, 226, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 48, 226, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 175, 0, 174, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 };
unsigned int generated_InvalidTexture_png_width = 16;
unsigned int generated_InvalidTexture_png_height = 16;
unsigned int generated_InvalidTexture_png_total_bytes = 1024;

unsigned char generated_StoneBlock_png[1024] = { 128, 128, 128, 255, 128, 128, 128, 255, 112, 112, 112, 255, 112, 112, 112, 255, 121, 121, 121, 255, 121, 121, 121, 255, 112, 112, 112, 255, 112, 112, 112, 255, 128, 128, 128, 255, 112, 112, 112, 255, 112, 112, 112, 255, 112, 112, 112, 255, 128, 128, 128, 255, 128, 128, 128, 255, 112, 112, 112, 255, 102, 102, 102, 255, 112, 112, 112, 255, 112, 112, 112, 255, 112, 112, 112, 255, 102, 102, 102, 255, 112, 112, 112, 255, 128, 128, 128, 255, 102, 102, 102, 255, 102, 102, 102, 255, 121, 121, 121, 255, 112, 112, 112, 255, 102, 102, 102, 255, 102, 102, 102, 255, 112, 112, 112, 255, 112, 112, 112, 255, 112, 112, 112, 255, 128, 128, 128, 255, 102, 102, 102, 255, 102, 102, 102, 255, 128, 128, 128, 255, 128, 128, 128, 255, 121, 121, 121, 255, 112, 112, 112, 255, 112, 112, 112, 255, 128, 128, 128, 255, 112, 112, 112, 255, 112, 112, 112, 255, 128, 128, 128, 255, 112, 112, 112, 255, 121, 121, 121, 255, 102, 102, 102, 255, 112, 112, 112, 255, 128, 128, 128, 255, 112, 112, 112, 255, 102, 102, 102, 255, 112, 112, 112, 255, 102, 102, 102, 255, 112, 112, 112, 255, 112, 112, 112, 255, 128, 128, 128, 255, 112, 112, 112, 255, 112, 112, 112, 255, 102, 102, 102, 255, 102, 102, 102, 255, 128, 128, 128, 255, 128, 128, 128, 255, 102, 102, 102, 255, 102, 102, 102, 255, 112, 112, 112, 255, 121, 121, 121, 255, 112, 112, 112, 255, 102, 102, 102, 255, 128, 128, 128, 255, 128, 128, 128, 255, 128, 128, 128, 255, 102, 102, 102, 255, 112, 112, 112, 255, 128, 128, 128, 255, 112, 112, 112, 255, 121, 121, 121, 255, 121, 121, 121, 255, 128, 128, 128, 255, 112, 112, 112, 255, 112, 112, 112, 255, 102, 102, 102, 255, 112, 112, 112, 255, 102, 102, 102, 255, 112, 112, 112, 255, 112, 112, 112, 255, 112, 112, 112, 255, 102, 102, 102, 255, 112, 112, 112, 255, 112, 112, 112, 255, 102, 102, 102, 255, 112, 112, 112, 255, 112, 112, 112, 255, 102, 102, 102, 255, 102, 102, 102, 255, 112, 112, 112, 255, 102, 102, 102, 255, 112, 112, 112, 255, 112, 112, 112, 255, 121, 121, 121, 255, 102, 102, 102, 255, 102, 102, 102, 255, 112, 112, 112, 255, 112, 112, 112, 255, 112, 112, 112, 255, 112, 112, 112, 255, 128, 128, 128, 255, 121, 121, 121, 255, 112, 112, 112, 255, 102, 102, 102, 255, 112, 112, 112, 255, 112, 112, 112, 255, 128, 128, 128, 255, 128, 128, 128, 255, 128, 128, 128, 255, 128, 128, 128, 255, 121, 121, 121, 255, 102, 102, 102, 255, 128, 128, 128, 255, 102, 102, 102, 255, 112, 112, 112, 255, 128, 128, 128, 255, 112, 112, 112, 255, 112, 112, 112, 255, 121, 121, 121, 255, 112, 112, 112, 255, 112, 112, 112, 255, 112, 112, 112, 255, 112, 112, 112, 255, 112, 112, 112, 255, 102, 102, 102, 255, 112, 112, 112, 255, 121, 121, 121, 255, 112, 112, 112, 255, 112, 112, 112, 255, 112, 112, 112, 255, 112, 112, 112, 255, 112, 112, 112, 255, 102, 102, 102, 255, 112, 112, 112, 255, 121, 121, 121, 255, 121, 121, 121, 255, 128, 128, 128, 255, 102, 102, 102, 255, 102, 102, 102, 255, 121, 121, 121, 255, 112, 112, 112, 255, 112, 112, 112, 255, 112, 112, 112, 255, 102, 102, 102, 255, 112, 112, 112, 255, 102, 102, 102, 255, 112, 112, 112, 255, 112, 112, 112, 255, 102, 102, 102, 255, 102, 102, 102, 255, 112, 112, 112, 255, 128, 128, 128, 255, 112, 112, 112, 255, 112, 112, 112, 255, 112, 112, 112, 255, 102, 102, 102, 255, 112, 112, 112, 255, 112, 112, 112, 255, 128, 128, 128, 255, 128, 128, 128, 255, 112, 112, 112, 255, 112, 112, 112, 255, 102, 102, 102, 255, 128, 128, 128, 255, 112, 112, 112, 255, 112, 112, 112, 255, 128, 128, 128, 255, 102, 102, 102, 255, 102, 102, 102, 255, 112, 112, 112, 255, 128, 128, 128, 255, 128, 128, 128, 255, 112, 112, 112, 255, 121, 121, 121, 255, 102, 102, 102, 255, 112, 112, 112, 255, 112, 112, 112, 255, 121, 121, 121, 255, 112, 112, 112, 255, 112, 112, 112, 255, 112, 112, 112, 255, 102, 102, 102, 255, 112, 112, 112, 255, 112, 112, 112, 255, 112, 112, 112, 255, 112, 112, 112, 255, 112, 112, 112, 255, 121, 121, 121, 255, 128, 128, 128, 255, 121, 121, 121, 255, 121, 121, 121, 255, 128, 128, 128, 255, 128, 128, 128, 255, 102, 102, 102, 255, 102, 102, 102, 255, 102, 102, 102, 255, 128, 128, 128, 255, 112, 112, 112, 255, 112, 112, 112, 255, 102, 102, 102, 255, 112, 112, 112, 255, 112, 112, 112, 255, 121, 121, 121, 255, 112, 112, 112, 255, 112, 112, 112, 255, 128, 128, 128, 255, 128, 128, 128, 255, 112, 112, 112, 255, 112, 112, 112, 255, 112, 112, 112, 255, 121, 121, 121, 255, 112, 112, 112, 255, 112, 112, 112, 255, 112, 112, 112, 255, 112, 112, 112, 255, 102, 102, 102, 255, 112, 112, 112, 255, 112, 112, 112, 255, 112, 112, 112, 255, 112, 112, 112, 255, 112, 112, 112, 255, 112, 112, 112, 255, 112, 112, 112, 255, 128, 128, 128, 255, 112, 112, 112, 255, 102, 102, 102, 255, 102, 102, 102, 255, 112, 112, 112, 255, 102, 102, 102, 255, 112, 112, 112, 255, 128, 128, 128, 255, 121, 121, 121, 255, 112, 112, 112, 255, 102, 102, 102, 255, 112, 112, 112, 255, 128, 128, 128, 255, 102, 102, 102, 255, 112, 112, 112, 255, 102, 102, 102, 255, 128, 128, 128, 255, 112, 112, 112, 255, 112, 112, 112, 255, 128, 128, 128, 255, 128, 128, 128, 255, 128, 128, 128, 255, 121, 121, 121, 255, 121, 121, 121, 255, 121, 121, 121, 255, 112, 112, 112, 255, 102, 102, 102, 255, 128, 128, 128, 255, 112, 112, 112, 255 };
unsigned int generated_StoneBlock_png_width = 16;
unsigned int generated_StoneBlock_png_height = 16;
unsigned int generated_StoneBlock_png_total_bytes = 1024;

