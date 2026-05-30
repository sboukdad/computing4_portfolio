/************************************************
 * Copyright Salma Boukdad
 * 
 * @Filename: TextWriter.cpp
 * @Course: COMP.2040.203
 * @Date: 5/28/26
 * @Description:
*************************************************/

#include <iostream>
#include <string>
#include "RandWriter.hpp"

int main(int argc, char* argv[]) {
    int k = std::stoi(argv[1]);
    int L = std::stoi(argv[2]);

    std::string text;

    // read all of stdin into text
    std::string line;
    while (std::getline(std::cin, line)) {
        text += line + "\n";
    }

    RandWriter rw(text, k);
    std::string seed = text.substr(0, k);
    std::cout << rw.generate(seed, L) << std::endl;

    return 0;
}