/*****************************************************
 * Copyright 2026 Salma Boukdad
 * @Author: Salma Boukdad
 * @Course: COMP.2040.203
 * @Date: 2/22/26
 * @Description: main source file for ps3a - creates:
 *      - Sokoban obj creation with filename
 *      - window sized to game dim.
 *      - event loop + close window handling 
 *****************************************************/

#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "Sokoban.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Proper entry: ./Sokoban <levelFile>"  << std::endl;
        return 1;
    }

    SB::Sokoban game(argv[1]);        // create Sokoban game with level sent in

    sf::RenderWindow window(sf::VideoMode(game.windowSize()), "Sokoban");

    // main game loop
    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        window.clear();
        window.draw(game);
        window.display();
    }
}
