#version 460 core

// inputs from vertex shader
in vec4 vertexColor;
in vec2 textureCoord;
in float textureLayer;

// uniforms
uniform sampler2DArray textureArray;
uniform sampler2D texture2d;

// outputs
out vec4 FragColor;

//#define TEST_TEXTURE_ARRAY
#define TEST_TEXTURE_2D

void SetFragColorTextureArray() 
{
    vec4 texColor = vec4(texture(textureArray, vec3(textureCoord, textureLayer)));
    FragColor = vertexColor * texColor;
}

void SetFragColorTexture2d()
{
    FragColor = vertexColor * vec4(texture(texture2d, textureCoord));
}

void main()
{
#ifdef TEST_TEXTURE_ARRAY
    SetFragColorTextureArray();
#elifdef TEST_TEXTURE_2D
    SetFragColorTexture2d();
#else
    //FragColor = vertexColor;
    SetFragColorTexture2d();
#endif
}

