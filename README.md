[![CMake](https://github.com/hilloftheking/briskbf/actions/workflows/cmake.yml/badge.svg)](https://github.com/hilloftheking/briskbf/actions/workflows/cmake.yml)
# About
briskbf is a basic C++ brainfuck interpreter that uses only the standard library. It takes an input file, tokenizes it, and executes it. Running Erik Bosman's mandel brainfuck script takes about 5.8 seconds to finish on my i3 10100.
There are 30,000 cells by default, but that can be customized by an argument. The pointer wraps around the cells if it goes below 0 or above the maximum amount. The cells are 1 byte and can also wrap.
# Building
briskbf can be built by using CMake to generate a makefile, or with Visual Studio.
