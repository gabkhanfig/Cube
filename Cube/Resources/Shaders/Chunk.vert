#version 460 core

// https://sight.pages.ircad.fr/sight-doc/CodingStyle/src/07-glsl-style.html

layout (location = 0) in vec3 v_in_position;
layout	(location = 1) in vec2 v_in_texCoord;

uniform mat4 u_cameraMVP;

// Interpolated fragment shader coordinates it's position in the triangle.
out vec3 v_out_fragCoord;
// Normalized coordinates of this vertex.
out flat vec3 v_out_vertCoord;
// Interpolated texture coordinates.
out vec2 v_out_texCoord;

#define SUBVOXEL_COUNT 16.0

#define BYTE_1_BITMASK 0xFF
#define BYTE_2_BITMASK 0xFF00
#define BYTE_3_BITMASK 0xFF0000
#define BYTE_4_BITMASK 0xFF000000

void main()
{
	//const vec3 position = PositionFromBitmask(aPos) / 255.0 - 0.5;
	gl_Position = u_cameraMVP * vec4(v_in_position, 1.0);

	v_out_vertCoord = v_in_position;
	v_out_fragCoord = v_in_position;
	v_out_texCoord = v_in_texCoord;
}