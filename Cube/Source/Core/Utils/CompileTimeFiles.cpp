#include "CompileTimeFiles.h"
#include "../../../Resources/GeneratedFiles.h"

// https://github.com/gabkhanfig/Cube-Compile-Time-File-Converter

HashMap<GlobalString, const CompileTimeFiles::Text*> CompileTimeFiles::textFiles;
HashMap<GlobalString, const CompileTimeFiles::Image*> CompileTimeFiles::imageFiles;

void CompileTimeFiles::LoadAllFiles()
{
	LoadTextFile("Chunk.vert"_str, generated_Chunk_vert);
	LoadTextFile("Chunk.frag"_str, generated_Chunk_frag);
	LoadTextFile("Block.vert"_str, generated_Block_vert);
	LoadTextFile("Block.frag"_str, generated_Block_frag);
	LoadTextFile("BlockMultidraw.vert"_str, generated_BlockMultidraw_vert);
	LoadTextFile("BlockMultidraw.frag"_str, generated_BlockMultidraw_frag);
	LoadTextFile("BlockPathtrace.vert"_str, generated_BlockPathtrace_vert);
	LoadTextFile("BlockPathtrace.frag"_str, generated_BlockPathtrace_frag);
	LoadTextFile("BlockPathtrace.comp"_str, generated_BlockPathtrace_comp);

	LoadImageFile("InvalidTexture.png"_str, generated_InvalidTexture_png, generated_InvalidTexture_png_width, generated_InvalidTexture_png_height, generated_InvalidTexture_png_total_bytes);
	LoadImageFile("StoneBlock.png"_str, generated_StoneBlock_png, generated_StoneBlock_png_width, generated_StoneBlock_png_height, generated_StoneBlock_png_total_bytes);
}

const CompileTimeFiles::Text* CompileTimeFiles::GetTextFile(GlobalString fileName)
{
	auto found = textFiles.find(fileName);
	gk_assertm(found != textFiles.end(), "Unable to find compile time text file");
	return found->second;
}

const CompileTimeFiles::Image* CompileTimeFiles::GetImageFile(GlobalString fileName)
{
	auto found = imageFiles.find(fileName);
	gk_assertm(found != imageFiles.end(), "Unable to find compile time text file");
	return found->second;
}

void CompileTimeFiles::LoadTextFile(GlobalString fileName, const char* contents)
{
	CompileTimeFiles::Text* text = new CompileTimeFiles::Text(contents);
	gk_assert(!textFiles.contains(fileName));
	textFiles.insert({ fileName, text });
}

void CompileTimeFiles::LoadImageFile(GlobalString fileName, uint8* contents, uint32 width, uint32 height, uint32 totalBytes)
{
	CompileTimeFiles::Image* image = new CompileTimeFiles::Image(contents, width, height, totalBytes);
	gk_assert(!imageFiles.contains(fileName));
	imageFiles.insert({ fileName, image });
}
