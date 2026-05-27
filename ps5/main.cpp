/***********************************************
 * Copyright Salma Boukdad
 * 
 * @Filename: main.cpp
 * @Course: COMP.2040.203
 * @Date: 5/22/26
 * @Description: Entry point for EDistance. This
 *  program accepts 
************************************************/

#include <iostream>
#include <stdexcept>
#include <string>
#include <SFML/System.hpp>
#include "EDistance.hpp"

int main() {
    sf::Clock clock;
    std::string s1, s2;
    std::getline(std::cin, s1);
    std::getline(std::cin, s2);
    if (!s1.empty() && s1.back() == '\r') s1.pop_back();
    if (!s2.empty() && s1.back() == '\r') s2.pop_back();

    EDistance distance(s1, s2);

    int editDistance = distance.optDistance();
    std::string aligned = distance.alignment();

    sf::Time t = clock.getElapsedTime();

    // print solution + running time
    std::cout << "Edit distance = " << editDistance << std::endl;
    std::cout << aligned;

    std::cout << "Execution time is " << t.asSeconds() << " seconds" <<
     std::endl;
}
