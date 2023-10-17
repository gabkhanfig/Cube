#include "InputMapping.h"
#include <GLFW/glfw3.h>

struct InputMapPair
{
	InputMapping::InputsString_T inputIds;
	InputMapping::InputsInt_T inputStrings;
};

static InputMapPair CreateDefaultInputs()
{
	InputMapPair inputs;

#define input(str, id) \
inputs.inputIds.insert({str, id}); \
inputs.inputStrings.insert({id, str})

	input("Space"_str, GLFW_KEY_SPACE);
	input("W"_str, GLFW_KEY_W);
	input("A"_str, GLFW_KEY_A);
	input("S"_str, GLFW_KEY_S);
	input("D"_str, GLFW_KEY_D);
	input("Esc"_str, GLFW_KEY_ESCAPE);
	input("Ctrl"_str, GLFW_KEY_LEFT_CONTROL);
	input("LMB"_str, GLFW_MOUSE_BUTTON_LEFT);
	input("RMB"_str, GLFW_MOUSE_BUTTON_RIGHT);
	input("MMB"_str, GLFW_MOUSE_BUTTON_MIDDLE);
	return inputs;
}

InputMapping::InputsString_T InputMapping::inputIds = CreateDefaultInputs().inputIds;
InputMapping::InputsInt_T InputMapping::inputStrings = CreateDefaultInputs().inputStrings;

GlobalString InputMapping::GetInputString(int id)
{
	auto found = inputStrings.find(id);
	if (found != inputStrings.end()) {
		return found->second;
	}
	return GlobalString();
}

int InputMapping::GetInputid(GlobalString str)
{
	auto found = inputIds.find(str);
	if (found != inputIds.end()) {
		return found->second;
	}
	return -1;
}

darray<GlobalString> InputMapping::GetMappedInputs()
{
	static InputMapping::InputsInt_T strs = CreateDefaultInputs().inputStrings;
	darray<GlobalString> mappedInputStrings;
	mappedInputStrings.Reserve(static_cast<uint32>(strs.size()));
	for (auto& str : strs) {
		mappedInputStrings.Add(str.second);
	}
	return mappedInputStrings;
}
