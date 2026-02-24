// Copyright 2026 Salma Boukdad
/*************************************************
 * @Author: Salma Boukdad
 * @Course: COMP.2040.202
 * @Date: 2/17/26
 * @Description:
 *************************************************/

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "PTree.hpp"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <L> <N>" << std::endl;
        return 1;
    }

    // parse command line
    float L = std::stof(argv[1]);
    int N = std::stoi(argv[2]);

    // create window
    sf::RenderWindow window(sf::VideoMode({static_cast<unsigned int>(6*L),
                                           static_cast<unsigned int>(4*L)}),
                                           "Pythagoras Tree");
    // create tree
    PTree tree(L, N);

    // draw loop
    while (window.isOpen()) {
        // handle events
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        // clear window
        window.clear(sf::Color::White);
        // draw the tree
        window.draw(tree);
        // display
        window.display();
    }
    return 0;
}
