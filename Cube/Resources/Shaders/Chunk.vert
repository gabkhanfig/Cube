#version 460 core

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
}