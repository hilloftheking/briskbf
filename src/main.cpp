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
#include <csignal>
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
	try {
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
				else {
					if (argPre == "-c") {
						unsigned int number = std::stoi(std::string{ arg });

						if (number <= 0) {
							throw std::invalid_argument{ "Cell count must be greater than 0." };
						}

						// Cell count
						CELL_AMOUNT = number;
					}
					else {
						throw std::invalid_argument{ std::string{"Unknown argument "} + argv[i] };
					}
				}
			}
			else {
				// File name, or help
				if (std::strcmp(argv[i], "-h") == 0 || std::strcmp(argv[i], "--help") == 0) {
					printHelp = true;
					break;
				}
				fileName = argv[i];
			}
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Error parsing arguments: " << e.what() << '\n';
		return 1;
	}

	if (printHelp) {
		std::cout << "Usage: bbf [options] [file]\n"
			<< "-h    print this menu.\n"
			<< "-c    cell amount. default=30000\n"
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
	std::vector<Tokenizer::Token> tokens;
	try {
		tokens = Tokenizer::tokenize(script);
	}
	catch (const std::exception& e) {
		std::cerr << "Invalid script: " << e.what() << '\n';
		return 1;
	}

	// Allocate the cells onto the heap
	try {
		cells = std::shared_ptr<unsigned char[]>(new unsigned char[CELL_AMOUNT]);
	}
	catch (...) {
		std::cerr << "Failed to allocate cells.\n";
		return 1;
	}
	std::memset(cells.get(), '\0', CELL_AMOUNT);
	pointer = 0;

	signal(SIGINT, [](int) {
		Interpreter::running = false;
	});

	// Execute the tokens
	auto start = std::chrono::high_resolution_clock::now();
	Interpreter::executeTokens(tokens);

	// Output execution time
	double time = std::chrono::duration<double, std::milli>(
		std::chrono::high_resolution_clock::now() - start).count();

	std::cout << "\nExecution finished in "
		<< time
		<< "ms \n";
}
