#pragma once

#include <string>
#include <vector>
#include "tokenizer.hpp"

namespace Interpreter {
	void executeTokens(const std::vector<Tokenizer::Token>& tokens);
};