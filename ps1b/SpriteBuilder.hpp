// Copyright 2026 Salma Boukdad
/*****************************************
 * @Author: Salma Boukdad
 * @Course: COMP.2040.203
 * @Date: 2/8/26
******************************************/

#pragma once

#ifndef SPRITEBUILDER_HPP
#define SPRITEBUILDER_HPP

#include <random>
#include <vector>
#include <string>

#include <SFML/Graphics.hpp>

#include "SpriteSheet.hpp"

namespace sb {
class SpriteBuilder : public sf::Drawable {
 private:
  // Fields go here
  std::vector<SpriteSheet> sheets;      // one sheet per layer, all in 1 vector
  std::vector<sf::Sprite> sprites;      // current tile from each sprite
  std::vector<sf::Texture> textures;    // textures for the sprites
  sf::Vector2u tileSize;                // size of each tile
  std::mt19937 rand;                    // random number generator
  mutable std::minstd_rand0 drawRand;   // const draw method

 protected:
  void draw(sf::RenderTarget &window, sf::RenderStates states) const override;

 public:
  explicit SpriteBuilder(const std::string &filename);         // random seed
  SpriteBuilder(const std::string &filename, uint32_t seed);   // fixed seed

  void randomize();                     // select new random parts
  sf::Vector2u windowSize() const;      // return tile size for window creation
};
}     // namespace sb

#endif
