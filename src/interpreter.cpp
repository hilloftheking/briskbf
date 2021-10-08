#include <iostream>
#include <stdexcept>
#include <string>
#include <exception>
#include <vector>
#include "globals.hpp"
#include "interpreter.hpp"
#include "tokenizer.hpp"

namespace Interpreter {
	// I would do using enum here, but g++10 does not support it.
	using namespace Tokenizer;

	// Executes a vector of tokens created from a brainfuck script
	void executeTokens(const std::vector<Tokenizer::Token>& tokens) {
		for (size_t i = 0; i < tokens.size(); i++) {
			const auto& tok = tokens[i];

			switch (tok.type) {
			case TokenType::increment:
				cells[pointer] += tok.data;
				break;
			case TokenType::decrement:
				cells[pointer] -= tok.data;
				break;
			case TokenType::moveright:
				pointer += tok.data;

				// Overflow check
				if (pointer >= CELL_AMOUNT) {
					pointer = pointer % CELL_AMOUNT;
				}

				break;
			case TokenType::moveleft:
				pointer -= tok.data;
				
				// Underflow check
				// Unsigned numbers wrap to max
				if (pointer >= CELL_AMOUNT) {
					// Subtract the amount to underflow from the maximum cell
					pointer = (CELL_AMOUNT - 1) - (UINT64_MAX - pointer);
					pointer %= CELL_AMOUNT; // double check that it is in bounds
				}

				break;
			case TokenType::output:
				for (int i = 0; i < tok.data; i++) {
					std::cout << cells[pointer];
				}
				break;
			case TokenType::input:
				for (int i = 0; i < tok.data; i++) {
					cells[pointer] = std::cin.get();
				}
				break;
			case TokenType::loopbegin:
				if (cells[pointer] == 0) {
					// skip loop
					i = tok.data; // data holds position of loop end
				}
				break;
			case TokenType::loopend:
				if (cells[pointer] != 0) {
					// continue loop
					i = tok.data; // data holds loop begin position
				}
				break;
			}
		}
	}
}