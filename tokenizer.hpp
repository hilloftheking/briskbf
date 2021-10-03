#pragma once

#include <vector>
#include <string>

namespace Tokenizer {
	enum class TokenType : int {
		increment,
		decrement,
		moveright,
		moveleft,
		output,
		input,
		loopbegin,
		loopend
	};

	struct Token {
		TokenType type;
		int data; // Repetitions for non loops, loop counterpart position for loops
	};

	std::vector<Token> tokenize(const std::string& script);
};