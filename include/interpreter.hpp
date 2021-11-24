#pragma once

#include <string>
#include <vector>
#include "tokenizer.hpp"

namespace Interpreter {
	extern bool running;

	void executeTokens(const std::vector<Tokenizer::Token>& tokens);
}