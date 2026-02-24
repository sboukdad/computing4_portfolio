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
  explicit SpriteBuilder(const std::string &filename);
  SpriteBuilder(const std::string &filename, uint32_t seed);

  sf::Vector2u windowSize() const;

  void randomize();

  // operator== overload
 protected:
  void draw(sf::RenderTarget &window, sf::RenderStates states) const override;

 private:
  // Fields go here
};
}     // namespace sb
