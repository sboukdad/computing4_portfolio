/**************************************************************************
 * Copyright Salma Boukdad & Jeannine Al Attal
 * @Authors: Salma Boukdad, Jeannine Al Attal
 * @Course: COMP.2040.203
 * @Date: 3/20/26
 * @Description: AniPlayer is the top-level class
 *    that connects a JSON animation file to the
 *    SFML window. It reads the window dimensions and delegates scene
 *    construction entirely to Component::fromJson()
 *************************************************************************/

#pragma once

#ifndef ANIPLAYER_HPP
#define ANIPLAYER_HPP

#include <vector>
#include <memory>
#include <string>
#include <SFML/Graphics.hpp>

#include "Component.hpp"

namespace AP {
class AniPlayer: public sf::Drawable {
 private:
  // fields go here
  unsigned int width;        // window width (in pixels) from JSON
  unsigned int height;       // window height (in pixels) from JSON

  // root of the component tree with type determined at runtime
  // unique_ptr owns the allocation and frees it automatically when
  // AniPlayer is destroyed
  std::unique_ptr<Component> scene;

 protected:
  // called automatically by window.draw(player), delegates to scene,
  // component tree root
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

 public:
  /**
   * Loads an AniPlayer containing the data from the given video
   * @param filename A .json file that contains data describing the video
   */
  explicit AniPlayer(const std::string &filename);

  /**
  * Advance the AniPlayer to the given time
  * @param time A given instance in time
  */
  void tween(sf::Time time);     // For part B

  /**
  * Gets the viewport dimensions for this Drawable
  * @return The size (in pixels)
  */
  sf::Vector2u windowSize() const;
};
}   // namespace AP
#endif
