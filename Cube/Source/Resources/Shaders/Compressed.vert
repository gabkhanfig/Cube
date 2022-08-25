#version 460 core

// Layout comes from VertexBufferLayout::MakeVertexBufferLayoutForVertex(). Formatting from vertex specification in Vertex.h
layout (location = 0) in uint vPos;
layout (location = 1) in uint vColor;
layout (location = 2) in uint vTexId;

// uniforms
uniform mat4 uMVP;
  
// Output to fragment shader
out vec4 vertexColor;
out vec2 textureCoord;
out float textureLayer;

//#define USE_MVP

void SetPosition() 
{
#ifdef USE_MVP
    //gl_Position = vec4(vPos, 1.0) * uMVP;
#else 
    //gl_Position = vec4(ExtractPosition(), 1.0);
#endif
}

vec4 ExtractColor() 
{
#define extractColorOffset(offset) float((vColor >> offset) & 255) / 255.f
    float r = extractColorOffset(0);
    float g = extractColorOffset(8);
    float b = extractColorOffset(16);
    float a = extractColorOffset(24);
    return vec4(r, g, b, a);
}

vec3 ExtractPosition() 
{
    vec3 chunkPos = vec3(1.f);
    float x = float(vPos & 255);
    float y = float((vPos >> 8) & 255);
    float z = float((vPos >> 16) & 255);
    return vec3(x * chunkPos.x, y * chunkPos.y, z * chunkPos.z);
}

vec2 ExtractUV()
{
    uint uvShift = vPos >> 24;
    float x = float(uvShift & 15) / 15.f;
    float y = float((uvShift >> 4) & 15) / 15.f;
    return vec2(x, y);
}

void main()
{
    gl_Position = vec4(ExtractPosition(), 1.0) * uMVP;
    vertexColor = ExtractColor();
    textureCoord = ExtractUV();
    textureLayer = float(vTexId);
}