#include <vector>
#include <optional>
#include <string>
#include "tokenizer.hpp"

namespace Tokenizer {
	std::vector<Token> tokenize(const std::string& script) {
		std::vector<Token> tokens;
		std::vector<int> loopBegin;

		char c;
		std::optional<TokenType> tt;
		Token* prevToken = (Token*)0;

		using enum TokenType;

		for (int i = 0; i < script.size(); i++) {
			c = script[i];

			switch (c) {
			case '+':
				tt = increment;
				break;
			case '-':
				tt = decrement;
				break;
			case '>':
				tt = moveright;
				break;
			case '<':
				tt = moveleft;
				break;
			case '.':
				tt = output;
				break;
			case ',':
				tt = input;
				break;
			case '[':
				tt = loopbegin;
				loopBegin.push_back(tokens.size()); // Push what the position of this opening bracket will be
				break;
			case ']':
				tt = loopend;
				break;
			default:
				tt.reset();
				break;
			}

			if (!tt.has_value()) {
				continue;
			}

			if (tt == loopbegin) {
				// The data in this will be filled out later by the loopend token.
				tokens.push_back(Token{ tt.value(), 0 });
			}
			else if (tt == loopend) {
				// Push back the loopend token, with data pointing to loopbegin.
				tokens.push_back(Token{ tt.value(), loopBegin.back() });
				// Set the loopbegin token's data to the position of this loopend, and pop the back of the loopBegin vector.
				tokens[loopBegin.back()].data = tokens.size() - 1;
				loopBegin.pop_back();
			}
			else if (prevToken && tt == prevToken->type) {
				// Increse repetitions.
				prevToken->data++;
			}
			else {
				// Push back new token with repetitions set to one
				tokens.push_back(Token{ tt.value(), 1 });
			}

			prevToken = &tokens.back();
		}

		return tokens;
	}
};