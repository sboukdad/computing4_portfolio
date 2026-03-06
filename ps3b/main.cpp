/*************************************************************
* Copyright 2026 Salma Boukdad
* @Author: Salma Boukdad
* @Course: COMP.2040.203
* @Date: 3/2/26
* @Description: Let's play Sokoban!
*************************************************************/

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Sokoban.hpp"
using SB::Sokoban;
using SB::Direction;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <level_file>" << std::endl;
        return 1;
    }

    Sokoban game(argv[1]);

    sf::RenderWindow window(sf::VideoMode(game.windowSize()), "Sokoban");

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* keyPressed =
                event->getIf<sf::Event::KeyPressed>()) {
                switch (keyPressed->code) {
                    case sf::Keyboard::Key::Up:
                    case sf::Keyboard::Key::W:
                        if (!game.isWon()) game.movePlayer(Direction::Up);
                        break;
                    case sf::Keyboard::Key::Down:
                    case sf::Keyboard::Key::S:
                        if (!game.isWon()) game.movePlayer(Direction::Down);
                        break;
                    case sf::Keyboard::Key::Left:
                    case sf::Keyboard::Key::A:
                        if (!game.isWon()) game.movePlayer(Direction::Left);
                        break;
                    case sf::Keyboard::Key::Right:
                    case sf::Keyboard::Key::D:
                        if (!game.isWon()) game.movePlayer(Direction::Right);
                        break;
                    case sf::Keyboard::Key::R:
                        game.reset();
                        break;
                    case sf::Keyboard::Key::Z:
                        game.undo();
                        break;
                    case sf::Keyboard::Key::Y:
                        game.redo();
                        break;
                    default:
                        break;
                }
            }
        }

        window.clear();
        window.draw(game);
        window.display();
    }

    return 0;
}
