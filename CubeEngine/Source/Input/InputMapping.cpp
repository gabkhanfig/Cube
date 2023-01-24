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

	input("Space", GLFW_KEY_SPACE);
	return inputs;
}

InputMapping::InputsString_T InputMapping::inputIds = CreateDefaultInputs().inputIds;
InputMapping::InputsInt_T InputMapping::inputStrings = CreateDefaultInputs().inputStrings;