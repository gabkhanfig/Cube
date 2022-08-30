#version 330 core

layout (location = 0) in uint aPos;
layout (location = 1) in uint colorData;
layout (location = 2) in uint texId; // First 30 bits are id value to index atlas. Second last bit is X coordinate. Last bit is the Y coordinate. X and Y are (0 - 1).

out vec4 attrib_fragment_color;
out vec2 v_TexCoord;
out float v_TexId;

uniform mat4 u_MVP; // model view projection matrix
uniform vec3 u_ChunkPosition;
uniform float u_AtlasSize; //= 16.f;
uniform float u_TextureSize; //= 16.f;

vec2 MapUVFromTextureId(vec2 uv, uint textureId) 
{
	float stepSize = u_TextureSize / u_AtlasSize;
	uint texRun = uint(u_AtlasSize / u_TextureSize);
	float texCount = texRun * texRun;

	//if(textureId >= texCount) {
	//	textureId = 0U;
	//}

	float x = stepSize * float(textureId % texRun);
	float y = stepSize * float(textureId / texRun);

	return vec2(x + (uv.x * stepSize), y + (uv.y * stepSize));
}

vec4 ExtractColorChannels(uint color) 
{
	float r = float(color & 255U) / 255.f;
	float g = float((color >> 8) & 255U) / 255.f;
	float b = float((color >> 16) & 255U) / 255.f;
	float a = float((color >> 24) & 255U) / 255.f;
	return vec4(r, g, b, a);
}

vec3 ExtractVertexWorldPosition(uint position) 
{
	float x = u_ChunkPosition.x + float(position & 255U);
	float y = u_ChunkPosition.y + float((position >> 8) & 255U);
	float z = u_ChunkPosition.z + float((position >> 16) & 255U);
	return vec3(x, y, z);
}

void main()
{
	attrib_fragment_color = ExtractColorChannels(colorData);
	vec4 asvec = vec4(ExtractVertexWorldPosition(aPos), 1.0);
	gl_Position = u_MVP * asvec;
	uint textureId = texId & 1073741823U; // Get last 30 bits.
	float x = (texId >> 30) & 1U;
	float y = (texId >> 31) & 1U;
	vec2 textureUVs = vec2(x, y);
	v_TexCoord = MapUVFromTextureId(textureUVs, textureId);
	v_TexId = texId;
}