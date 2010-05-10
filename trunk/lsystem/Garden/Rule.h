#pragma once

#include "StaticString.h"

namespace AP_LSystem {
//struct StaticString;
struct Rule
{
	FunctionParser* condition;
	std::string variables;
	int * stochasticIndex;
	std::vector<StaticString> staticStrings;
	std::vector<FunctionParser*> dynamicStrings;
};
}
