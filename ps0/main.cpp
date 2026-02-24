/*****************************************************************************
 * Copyright 2025 Salma Boukdad
 * @Author: Salma Boukdad
 * @Filename: main.cpp
 * @Project: PS0
 * @Course: COMP.2040.203
 * @Date: 1/26/26
 * @Description: This program is an introduction to SFML. This is a library 
 *  that allows us to create programs or applications provided the interface 
 *  for graphics, audio, window management, etc. This program produces a window
 *  with a white circle and a bowl of ramen in the middle. This ramen (sprite) 
 *  can move via keyboard up, down, left, right keys. There is also a soothing 
 *  audio that goes with the image featured in this example.
 ****************************************************************************/

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

int main() {
    // create window and set dimensions
    sf::RenderWindow window(sf::VideoMode({200, 200}), "SFML works!");
    sf::CircleShape shape(100.f);                        // create shape
    sf::Texture texture;                                  // create texture

    // condition if sprite texture does not load
    if (!texture.loadFromFile("sprite.png")) {
        return EXIT_FAILURE;
    }

    sf::Sprite sprite(texture);                  // sprite constructor
    sprite.setTexture(texture);  // set sprite texture (position (0,0) default)

    shape.setFillColor(sf::Color::White);            // set shape color

    // load music to play
    sf::Music music;                                     // create music
    if (!music.openFromFile("ramenShop.ogg")) {
        return EXIT_FAILURE;
    }

    music.play();                                       // play the music

    // center sprite in window
    sf::FloatRect spriteBounds = sprite.getLocalBounds();
    sprite.setOrigin({spriteBounds.size.x / 2, spriteBounds.size.y / 2});
    sprite.setPosition({100.f, 100.f});                // center of window

    // start the game loop
    while (window.isOpen()) {
        window.setFramerateLimit(60);            // set window FPS

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {  // closed
                window.close();
            }
        }

        // desired static size
        float targetWidth = 150.f;
        float targetHeight = 200.f;

        sf::Vector2u imgSize = texture.getSize();  // get original texture size

        // scale sprite to target size
        sprite.setScale({targetWidth / imgSize.x, targetHeight / imgSize.y});

        // move sprite with keyboard input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
            sprite.move({0, -5});  // move up
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
            sprite.move({0, 5});  // move down
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            sprite.move({-5, 0});  // move left
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            sprite.move({5, 0});  // move right
        }
        
        window.clear();
        window.draw(shape);
        window.draw(sprite);
        window.display();
    }
    return 0;
}