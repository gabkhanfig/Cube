#pragma once

#include "../EngineCore.h"

class InputMapping
{
public:

		typedef std::unordered_map<GlobalString, int> InputsString_T;
		typedef std::unordered_map<int, GlobalString> InputsInt_T;

private:

	/* Initialized on application startup. */
	static InputsString_T inputIds;
	static InputsInt_T inputStrings;

public:

	/**/
	static GlobalString GetInputString(int id);

	/**/
	static int GetInputid(GlobalString str);

	/**/
	static darray<GlobalString> GetMappedInputs();

	
};