#include <vector>
#include <optional>
#include <fstream>
#include "tokenizer.hpp"

namespace Tokenizer {
	std::vector<Token> tokenize(std::ifstream& script) {
		std::vector<Token> tokens;
		std::vector<int> loopBegin;

		char c;
		std::optional<TokenType> tt;
		Token* prevToken = NULL;

		// I would do using enum here, but g++10 does not support it.

		while (script.good()) {
			script.get(c);

			switch (c) {
			case '+':
				tt = TokenType::increment;
				break;
			case '-':
				tt = TokenType::decrement;
				break;
			case '>':
				tt = TokenType::moveright;
				break;
			case '<':
				tt = TokenType::moveleft;
				break;
			case '.':
				tt = TokenType::output;
				break;
			case ',':
				tt = TokenType::input;
				break;
			case '[':
				tt = TokenType::loopbegin;
				loopBegin.push_back(tokens.size()); // Push what the position of this opening bracket will be
				break;
			case ']':
				tt = TokenType::loopend;
				break;
			default:
				tt.reset();
				break;
			}

			if (!tt.has_value()) {
				continue;
			}

			if (tt == TokenType::loopbegin) {
				// The data in this will be filled out later by the loopend token.
				tokens.push_back(Token{ tt.value(), 0 });
			}
			else if (tt == TokenType::loopend) {
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
}