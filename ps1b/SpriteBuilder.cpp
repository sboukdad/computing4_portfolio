// Copyright 2026 Salma Boukdad
/**********************************
 * @Author: Salma Boukdad
 * @Course: COMP.2040.203
 * @Date: 2/8/26
 **********************************/

#include "SpriteBuilder.hpp"
#include <string>
#include <utility>
#include <fstream>

namespace sb {
// generate random seed from current time
SpriteBuilder::SpriteBuilder(const std::string &filename)
: SpriteBuilder(filename,
std::chrono::system_clock::now().time_since_epoch().count()) {}

SpriteBuilder::SpriteBuilder(const std::string &filename, uint32_t seed)
: rand(seed) {
    std::ifstream file(filename);

    // read first line: tileWidth, tileHeight, margin
    unsigned int width, height, margin;
    file >> width >> height >> margin;
    tileSize = sf::Vector2u(width, height);

    // read each spriteSheet filename and load it
    std::string sheetFilename;
    while (file >> sheetFilename) {
        sf::Image img(sheetFilename);
        sheets.emplace_back(img, tileSize, margin);
    }

    // pick random tiles for first sprite
    randomize();
    randomize();
}

// draw all layered sprites
void SpriteBuilder::draw(sf::RenderTarget &window, sf::RenderStates states)
const {
    for (const auto& sprite : sprites) {
        window.draw(sprite, states);
    }
}

// prick new random tiles from each sheet
void SpriteBuilder::randomize() {
    sprites.clear();
    textures.clear();

    for (const auto& sheet : sheets) {
        // pick random tile index
        std::uniform_int_distribution<size_t> dist(0, sheet.length() - 1);
        size_t randomIndex = dist(rand);

        // get the tile as a TexureView
        TextureView view = sheet[randomIndex];
        sf::Image img = view.toImage();

        // make white pixels transparent
        img.createMaskFromColor(sf::Color::White);

        // create texture and load image
        sf::Texture texture;
        if (!texture.loadFromImage(img)) {
            throw std::runtime_error("Failed to load texture from image");
        }
        textures.push_back(std::move(texture));     // move into vector

        // create sprite using the texture we just added
        sf::Sprite sprite(textures.back());
        sprites.push_back(sprite);
    }
}

// return the size of one tile
sf::Vector2u SpriteBuilder::windowSize() const {
    return tileSize;
}
}    // namespace sb
