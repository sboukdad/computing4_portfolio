// Copyright 2026 Salma Boukdad
/*****************************************
 * @Author: Salma Boukdad
 * @Course: COMP.2040.203
 * @Date: 2/8/26
******************************************/

#include <stdexcept>
#include "SpriteSheet.hpp"

namespace sb {
// parameterized constructor
TextureView::TextureView(std::shared_ptr<sf::Texture> texture,
unsigned int x, unsigned int y, unsigned int width, unsigned int height) :
texture(texture), x(x), y(y), width(width), height(height) {}

// create new TextureView that shows a portion of the Txtr relative to the view
TextureView TextureView::crop(sf::IntRect area) const {
    // validate crop region is within view bounds
    if (area.position.x < 0 || area.position.y < 0) {
        throw std::out_of_range("Crop position cannot be negative");
    }

    if (area.position.x + static_cast<unsigned int>(area.size.x) > this->width
        || area.position.y + static_cast<unsigned int>(area.size.y) >
            this->height) {
        throw std::out_of_range("Crop region extends beyond view bounds");
    }

    // check texture is not null before passing it on
    if (!texture) {
        throw std::runtime_error("Can not crop a view without a texture");
    }

    // calculate new cropped region in org texture
    unsigned int newX = this->x + static_cast<unsigned int>(area.position.x);
    unsigned int newY = this->y + static_cast<unsigned int>(area.position.y);

    // create new view sharing same texture
    return TextureView(this->texture, newX, newY,
        static_cast<unsigned int>(area.size.x),
        static_cast<unsigned int>(area.size.y));
}

// get width and height of view (region only, not full img)
sf::Vector2u TextureView::getSize() const {
    return sf::Vector2u({width, height});
}

// create new Sprite obj initialized to contain portion of Texture in view
sf::Sprite TextureView::toSprite() const {
    // define rectangle of view and return it
    sf::IntRect view({static_cast<int>(x), static_cast<int>(y)},
        {static_cast<int>(width), static_cast<int>(height)});
    return sf::Sprite(*texture, view);
}

// create new sf::Image from the portion of the texture in view
sf::Image TextureView::toImage() const {
    sf::Image fullImg = texture->copyToImage();       // get full img
    sf::Image subImg({width, height});          // new img of view

    // create tile of view
    sf::IntRect tile({static_cast<int>(x), static_cast<int>(y)},
        {static_cast<int>(width), static_cast<int>(height)});

    // if not sumImg
    if (!subImg.copy(fullImg, {0, 0}, tile)) {
        throw std::runtime_error("Tile did not load properly");
    }
    return subImg;
}
}       // namespace sb
