#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>
#include <chrono>
#include <ratio>
#include <cstring>
#include <exception>
#include "globals.hpp"
#include "interpreter.hpp"
#include "tokenizer.hpp"

// This can be changed through arguments
unsigned int CELL_AMOUNT = 30'000;

std::shared_ptr<unsigned char[]> cells;
unsigned int pointer;

int main(int argc, char* argv[]) {
	std::string fileName;

	// Parse arguments
	bool printHelp = false;
	for (int i = 1; i < argc; i++) {
		// Process arguments before last (file name)
		if (i != argc - 1) {
			auto argSize = strlen(argv[i]);

			// Argument prefix
			std::string_view argPre{ argv[i] };
			argPre.remove_suffix(argSize - 2);

			// Argument value
			std::string_view arg{ argv[i] };
			arg.remove_prefix(2);

			if (argPre == "-h") {
				// Help
				printHelp = true;
				break;
			}
			else if (argPre == "-i") {
				// Disabled for now
				// Custom std::cin
				//auto* input = new std::stringstream;
				//*input << arg << '\n';

				//std::cin.set_rdbuf(input->rdbuf());
			}
			else {
				// These arguments take a number parameter.
				unsigned int number;

				try {
					number = std::stoul(std::string{ arg });
				}
				catch (const std::exception& e) {
					std::cerr << "Invalid argument: " << e.what() << '\n';
					return 1;
				}

				if (argPre == "-c") {
					// Cell count
					CELL_AMOUNT = number;
				}
				else {
					std::cerr << "Invalid argument.\n";
					return 1;
				}
			}
		}
		else {
			// File name, or help
			if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
				printHelp = true;
				break;
			}
			fileName = argv[i];
		}
	}

	if (printHelp) {
		std::cout << "Usage: bbf [options] [file]\n"
			<< "-h    print this menu.\n"
			<< "-c    cell amount. default=30000\n"
			// << "-i    write custom characters to std::cin for debugging\n"
			<< "\nExample: bbf -c1000 mandel.bf\n";

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
		return 1;
	}

	// Tokenize script for much better performance
	auto tokens = Tokenizer::tokenize(script);

	// Allocate the cells onto the heap
	cells = std::shared_ptr<unsigned char[]>(new unsigned char[CELL_AMOUNT]);
	memset(cells.get(), 0, CELL_AMOUNT);
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

	double time = std::chrono::duration<double, std::milli>(
		std::chrono::high_resolution_clock::now() - start).count();

	// Output execution time
	std::cout << "\nExecution finished in "
		<< time
		<< "ms \n";
}
