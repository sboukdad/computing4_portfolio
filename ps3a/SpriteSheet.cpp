/************************************************************************************
 * Copyright 2026 Salma Boukdad
 * @Author: Salma Boukdad
 * @Course: COMP.2040.203
 * @Date: 2/8/26
 * @Description: The SpriteSheet class knows how to extract tiles from a tileSheet
************************************************************************************/

#include <stdexcept>
#include <string>
#include <memory>

#include "SpriteSheet.hpp"

namespace sb {

SpriteSheet::SpriteSheet(sf::Image img, sf::Vector2u tileSize,
    int margin) : img(img), tileSize(tileSize), margin(margin) {

    texture = std::make_shared<sf::Texture>();
    if (!texture->loadFromImage(img)) {
        throw std::runtime_error("Can not load texture from image");
    }

    sf::Vector2u textureSize = texture->getSize();  // get texture dimensions

    // calculate length, width, height
    _width = (textureSize.x + margin) / (tileSize.x + margin);
    _height = (textureSize.y + margin) / (tileSize.y + margin);
    _length = _width * _height;
    tileSheet = sf::Vector2u(_width, _height);
}

SpriteSheet::SpriteSheet(const std::string& imgFilename, sf::Vector2u tileSize,
    int margin) : imgFilename(imgFilename), tileSize(tileSize),
     margin(margin) {
    texture = std::make_shared<sf::Texture>();      // allocate memory

    // throw exception if !imgFilename
    if (!texture->loadFromFile(imgFilename)) {
        throw std::runtime_error(
            "Could not find spritesheet at " + imgFilename);
    }

    sf::Vector2u textureSize = texture->getSize();

    _width = (textureSize.x + margin) / (tileSize.x + margin);
    _height = (textureSize.y + margin) / (tileSize.y + margin);
    _length = _width * _height;
    tileSheet = sf::Vector2u(_width, _height);
}

sf::Vector2u SpriteSheet::getTileSize() const {
    return tileSize;
}

// return tile count dimensions
sf::Vector2u SpriteSheet::size() const {
    return sf::Vector2u(_width, _height);
}

size_t SpriteSheet::width() const {
    return _width;
}

size_t SpriteSheet::height() const {
    return _height;
}

size_t SpriteSheet::length() const {
    return _length;
}

// return TextureView using [col, row] index
TextureView SpriteSheet::operator[](sf::Vector2u pt) const {
    // check bounds
    if (pt.x >= _width || pt.y >= _height) {
        throw std::out_of_range("Tile coordinates out of bounds");
    }

    unsigned int posX = (tileSize.x + margin) * pt.x;
    unsigned int posY = (tileSize.y + margin) * pt.y;

    return TextureView(texture, posX, posY, tileSize.x, tileSize.y);
}

// return TextureView using [i] grid index
TextureView SpriteSheet::operator[](size_t i) const {
    // check bounds
    if (i >= _length) {
        throw std::out_of_range("Tile index out of bounds");
    }

    unsigned int col = i % _width;
    unsigned int row = i / _width;

    return (*this)[sf::Vector2u(col, row)];
}
}       // namespace sb
