// Copyright 2026 Salma Boukdad
/*****************************************
 * @Author: Salma Boukdad
 * @Course: COMP.2040.203
 * @Date: 2/10/26
******************************************/

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "SpriteBuilder.hpp"

// parse command line arguments
struct Config {
    std::string filename;
    int scale = 1;
    uint32_t seed = 0;
    bool hasSeed = false;
};

Config parseArgs(int argc, char* argv[]) {
    Config config;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg.find("--file=") == 0) {
            config.filename = arg.substr(7);             // remove "--file="
        } else if (arg.find("--scale=") == 0) {
            config.scale = std::stoi(arg.substr(8));     // remove "--scale="
        } else if (arg.find("--seed=") == 0) {
            config.seed = std::stoul(arg.substr(7));     // remove "--seed="
            config.hasSeed = true;
        }
    }

    if (config.filename.empty()) {
        throw std::runtime_error("--file parameter is required");
    }

    return config;
}

int main(int argc, char* argv[]) {
    try {
        // parse command line args
        Config config = parseArgs(argc, argv);

        // create spritebuilder
        sb::SpriteBuilder builder = config.hasSeed
         ? sb::SpriteBuilder(config.filename, config.seed)
         : sb::SpriteBuilder(config.filename);

        // get window size with padding
        sf::Vector2u tileSize = builder.windowSize();
        unsigned int padding = 50;
        sf::Vector2u windowSize(tileSize.x * config.scale + padding * 2,
                                tileSize.y * config.scale + padding * 2);

        // create window
        sf::RenderWindow window(sf::VideoMode(windowSize), "SpriteBuilder");

        // counter for saved images
        int saveCounter = 0;

        // main loop
        while (window.isOpen()) {
            while (auto event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) {
                    window.close();
                }

                if (const auto* keyPress =
                    event->getIf<sf::Event::KeyPressed>()) {
                    // R key - randomize
                    if (keyPress->code == sf::Keyboard::Key::R) {
                        builder.randomize();
                    }

                    // S key - save
                    if (keyPress->code == sf::Keyboard::Key::S) {
                        sf::RenderTexture renderTexture;
                        if (!renderTexture.resize(tileSize)) {
                            std::cerr << "Failed to create render texture" <<
                            std::endl;
                            continue;
                        }

                        renderTexture.clear(sf::Color::Transparent);
                        renderTexture.draw(builder);
                        renderTexture.display();

                        // save to file
                        sf::Image img =
                        renderTexture.getTexture().copyToImage();

                        // extract base filename
                        std::string baseName = config.filename;
                        size_t lastSlash = baseName.find_last_of("/\\");
                        if (lastSlash != std::string::npos) {
                            baseName = baseName.substr(lastSlash + 1);
                        }

                        size_t dotPos = baseName.find_last_of('.');
                        if (dotPos != std::string::npos) {
                            baseName = baseName.substr(0, dotPos);
                        }

                        // create filename with counter
                        std::string filename = baseName + "_" +
                            (saveCounter < 10 ? "0" : "") +
                            std::to_string(saveCounter) + ".png";

                        if (img.saveToFile(filename)) {
                            std::cout << "Saved to " << filename << std::endl;
                            saveCounter++;
                        } else {
                            std::cerr << "Failed to save image" << std::endl;
                        }
                    }
                }
            }

            // clear and draw
            window.clear(sf::Color::Black);

            // draw with scaling and padding offset
            sf::RenderStates states;
            sf::Vector2f paddingOffset(static_cast<float>(padding),
                                       static_cast<float>(padding));
            states.transform.translate(paddingOffset);
            states.transform.scale({static_cast<float>(config.scale),
                                    static_cast<float>(config.scale)});

            window.draw(builder, states);

            window.display();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
