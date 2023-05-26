#pragma once

#include "../EngineTypes.h"

class CompileTimeFiles
{
public:

	struct Text {
		const char* contents;

		Text(const char* _c) : contents(_c) {}
	};

	struct Image {
		const uint8* contents;
		const uint32 width;
		const uint32 height;
		const uint32 totalBytes;

		Image(const uint8* _contents, uint32 _width, uint32 _height, uint32 _totalBytes) :
			contents(_contents), width(_width), height(_height), totalBytes(_totalBytes) {}
	};

public:

	/* Load all of the compile time files into properly structured data. */
	static void LoadAllFiles();

	/* Get a text file structure. Must exist. */
	static const Text* GetTextFile(GlobalString fileName);

	/* Get an image file structure. Must exist. */
	static const Image* GetImageFile(GlobalString fileName);

private:

	static void LoadTextFile(GlobalString fileName, const char* contents);

	static void LoadImageFile(GlobalString fileName, uint8* contents, uint32 width, uint32 height, uint32 totalBytes);

private:

	static HashMap<GlobalString, const Text*> textFiles;

	static HashMap<GlobalString, const Image*> imageFiles;

};