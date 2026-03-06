// Copyright 2026 Salma Boukdad

#pragma once

#include <random>
#include <vector>
#include <string>

#include <SFML/Graphics.hpp>

#include "SpriteSheet.hpp"

namespace sb {
class SpriteBuilder : public sf::Drawable {
 public:

  // read a config file that lists sprite sheet files
  explicit SpriteBuilder(const std::string &filename);

  // reads file and randomly picks one tile from each sheet
  // layers them on tops of each other to complete character
  SpriteBuilder(const std::string &filename, uint32_t seed);

  sf::Vector2u windowSize() const;

  // generate new combo when calledok
  void randomize();

  // operator== overload
 protected:
  void draw(sf::RenderTarget &window, sf::RenderStates states) const override;

 private:
  // Fields go here
};
}     // namespace sb
