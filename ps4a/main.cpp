/***************************************************
 * Copyright Salma Boukdad & Jeannine Al Attal
 * @Authors: Salma Boukdad, Jeannine Al Attal
 * @Course: COMP.2040.203
 * @Date: 3/21/26
 * @Description: Entry point for AniPlayer. This
 *    file accepts a JSON animation file as a 
 *    command-line argument and plays it,
 *    using Space/P for pause/resume,
 *    and R for restart
 **************************************************/
#include <iostream>
#include <stdexcept>
#include <SFML/Graphics.hpp>
#include "AniPlayer.hpp"

int main(int argc, char* argv[]) {
    // take in one argument, the JSON filename
    if (argc != 2) {
        // print usage and exit instead of crashing
        std::cerr << "Format: AniPlayer <filename>\n";
        return 1;
    }

    // draw empty AniPlayer to the window
    AP::AniPlayer player(argv[1]);

    // create emtpy window with specified size
    sf::RenderWindow window(sf::VideoMode(player.windowSize()), "AniPlayer");

    // clock for Part B
    sf::Clock timer;

    // main render loop
    while (window.isOpen()) {
        // handle event
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {       // if closed, exit loop
                window.close();
            }

            // keyboard shortcuts
            const auto* key = event->getIf<sf::Event::KeyPressed>();
            if (key != nullptr) {
                switch (key->code) {
                    // if R is pressed - restart animation from the beginning
                    case sf::Keyboard::Key::R:
                        timer.reset();
                        timer.start();
                        break;
                    // if Space or P is pressed - pause/resume
                    case sf::Keyboard::Key::Space:
                    case sf::Keyboard::Key::P:
                        if (timer.isRunning())
                            timer.stop();
                        else
                            timer.start();
                        break;
                    default:
                        break;
                }
            }
        }

        // update for part B
        // player.tween(timer.getElapsedTime());

        // render
        window.clear(sf::Color::Black);        // black background
        window.draw(player);                   // get full component tree
        window.display();
    }
    return 0;
}
