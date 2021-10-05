#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <chrono>
#include <ratio>
#include <cstring>
#include <exception>
#include "globals.hpp"
#include "interpreter.hpp"
#include "tokenizer.hpp"

// This can be changed through arguments
uint64_t CELLS_SIZE = 30'000;

std::shared_ptr<unsigned char[]> cells;
uint64_t pointer;

int main(int argc, char* argv[]) {
	std::string fileName;

	// Parse arguments
	bool printHelp = false;
	for (int i = 1; i < argc; i++) {
		// Process arguments before last (file name)
		if (i != argc - 1) {
			std::string arg{ argv[i], 0, 2 };

			if (arg == "-h") {
				printHelp = true;
				break;
			}
			else {
				int number = std::stoi(std::string{ argv[i], 2, strlen(argv[i]) });

				if (arg == "-c") {
					CELLS_SIZE = number;
				}
				else if (arg == "-s") {
					// TODO: More character types
				}
				else {
					std::cerr << "Invalid argument.\n";
					return 1;
				}

				std::cout << number << '\n'; // Debug purposes
			}
		}
		else {
			// File name, or help
			if (strcmp(argv[i], "-h") == 0) {
				printHelp = true;
				break;
			}
			fileName = argv[i];
		}
	}

	if (printHelp) {
		std::cout << "Usage: bf-interpret [options] [file]\n"
			<< "-h    print this menu.\n"
			<< "-c    cell amount. default = 30000\n"
			<< "-s    cell size (8, 16, 32). default = 8\n"
			<< "\nExample: bf-interpret -c20 -s32 helloworld.bf\n";

		return 1;
	}

	if (fileName == "") {
		// Allow user to input file
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

	try {
		Interpreter::executeTokens(tokens);
	}
	catch (const std::exception& e) {
		std::cerr << "Execution failed: " << e.what() << '\n';
		return 1;
	}

	// Output execution time
	std::cout << "\nExecution finished in "
		<< std::chrono::duration<double, std::milli>(
			std::chrono::high_resolution_clock::now() - start)
		<< '\n';
}