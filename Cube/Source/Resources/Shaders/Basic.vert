#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 colorLocation;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in float texId;

out vec4 attrib_fragment_color;
out vec2 v_TexCoord;
out float v_TexId;

uniform mat4 u_MVP; // model view projection matrix

void main()
{
	attrib_fragment_color = vec4(colorLocation.x, colorLocation.y, colorLocation.z, colorLocation.w);
	vec4 asvec = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	gl_Position = u_MVP * asvec;
	v_TexCoord = texCoord;
	v_TexId = texId;
}