#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <chrono>
#include <ratio>
#include "globals.hpp"
#include "interpreter.hpp"
#include "tokenizer.hpp"

const uint64_t CELLS_SIZE = 30'000;

std::shared_ptr<unsigned char[]> cells;
uint64_t pointer;

int main(int argc, char* argv[]) {
	std::string fileName;
	if (argc > 1) { // argv always has the program name
		fileName = argv[1];
	}
	else {
		std::cout << "File name: ";
		std::cin >> fileName;
	}

	std::ifstream script{ fileName };

	if (!script.is_open()) {
		std::cerr << "File " << fileName << " not found. Aborting.\n";
		return -1;
	}

	// Tokenize script for much better performance
	auto tokens = Tokenizer::tokenize(script);

	// Allocate the cells onto the heap
	cells = std::shared_ptr<unsigned char[]>(new unsigned char[CELLS_SIZE]);
	memset(cells.get(), 0, CELLS_SIZE);
	pointer = 0;

	// Execute the tokens
	auto start = std::chrono::high_resolution_clock::now();
	Interpreter::executeTokens(tokens);

	// Output execution time
	std::cout << "\nExecution finished in "
		<< std::chrono::duration<double, std::milli>(
			std::chrono::high_resolution_clock::now() - start) << '\n';
}