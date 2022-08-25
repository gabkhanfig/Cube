#version 330 core

out vec4 FragColor;

in vec4 attrib_fragment_color;
in vec2 v_TexCoord;
in float v_TexId;

uniform sampler2D u_Texture;
uniform sampler2DArray textureArray;

void main()
{
	// 2d texture
	//FragColor = texture(u_Texture, v_TexCoord);

	// texture arrays
	vec4 texColor = vec4(texture(textureArray, vec3(v_TexCoord.xy, v_TexId)));
	FragColor = vec4(texColor.x * attrib_fragment_color.x, texColor.y * attrib_fragment_color.y, texColor.z * attrib_fragment_color.z,texColor.w * attrib_fragment_color.w);

	// texture coords
	//FragColor = vec4(v_TexCoord.x, v_TexCoord.y, 1, 1);
}

