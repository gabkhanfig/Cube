#version 460 core

// https://sight.pages.ircad.fr/sight-doc/CodingStyle/src/07-glsl-style.html

//layout (location = 0) in uint v_in_packedOffsetPosition; // PackedBlockOffsetPosition struct
layout (location = 0) in vec3 v_in_offsetPosition; // glm::vec3
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
	//const vec3 vertexPosition = UnpackOffsetPosition(v_in_packedOffsetPosition);
	//gl_Position = u_cameraMVP * vec4(vertexPosition + u_chunkOffset, 1.0);
	gl_Position = u_cameraMVP * vec4(v_in_offsetPosition + u_chunkOffset, 1.0);

	v_out_texCoord = v_in_texCoord;
	v_out_color = UnpackColor(v_in_packedColor);

	v_out_normalColorScale = ScaleColorByNormal(UnpackNormal(v_in_packedNormal));
}