/*****************************************
 * Copyright 2026 Salma Boukdad
 * @Author: Salma Boukdad
 * @Course: COMP.2040.203
 * @Date: 2/8/26
******************************************/

#pragma once

#ifndef SPRITESHEET_HPP
#define SPRITESHEET_HPP


#include <string>
#include <memory>
#include <SFML/Graphics.hpp>


namespace sb {
// TextureView represents a sub-image of Texture object
class TextureView {
 public:
  TextureView();
  TextureView(std::shared_ptr<sf::Texture> texture, unsigned int x,
      unsigned int y, unsigned int width, unsigned int height);

  TextureView crop(sf::IntRect area) const;
  sf::Vector2u getSize() const;
  sf::Sprite toSprite() const;
  sf::Image toImage() const;

 private:
  std::shared_ptr<sf::Texture> texture;     // shared_ptr object to texture
  unsigned int x, y,                        // coord where view begins in txtre
    width, height;                          // dimensions of new view
};

/*****************************************************************************/

// SpriteSheet will know how to select sub-images from a Texture
class SpriteSheet {
 public:
  SpriteSheet(sf::Image img, sf::Vector2u tileSize, int margin = 0);
  SpriteSheet(const std::string &imgFilename, sf::Vector2u tileSize,
   int margin = 0);

  // getters
  sf::Vector2u getTileSize() const;
  sf::Vector2u size() const;        // the size of the view
  size_t length() const;
  size_t width() const;
  size_t height() const;

  // index operator overloading to identify correct TextureView obj
  TextureView operator[](sf::Vector2u pt) const;      // get TV by coordinates
  TextureView operator[](size_t i) const;             // get TV by index

 private:
  // Fields go here
  sf::Image img;
  const std::string imgFilename;
  std::shared_ptr<sf::Texture> texture;  // create Texture and store in shr_ptr
  sf::Vector2u tileSize;                 // dimensions of 1 tile
  sf::Vector2u tileSheet;                // (x tiles long, Y tiles high)
  std::size_t _length, _width, _height;
  int margin;
};
}        // namespace sb
#endif
