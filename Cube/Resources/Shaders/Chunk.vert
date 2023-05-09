#version 460 core

// https://sight.pages.ircad.fr/sight-doc/CodingStyle/src/07-glsl-style.html

layout (location = 0) in vec3 v_in_position;
layout (location = 1) in vec3 v_in_normal;
layout (location = 2) in vec2 v_in_texCoord;
layout (location = 3) in vec3 v_in_color;

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
out vec3 v_out_color;
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

void main()
{
	gl_Position = u_cameraMVP * vec4(v_in_position + u_chunkOffset, 1.0);

	v_out_texCoord = v_in_texCoord;
	v_out_color = v_in_color;

	v_out_normalColorScale = ScaleColorByNormal(v_in_normal);
}