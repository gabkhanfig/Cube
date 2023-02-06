#version 460 core

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
}