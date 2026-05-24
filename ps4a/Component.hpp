/***************************************************
 * Copyright Salma Boukdad & Jeannine Al Attal
 * @Authors: Salma Boukdad, Jeannine Al Attal
 * @Course: COMP.2040.203
 * @Date: 3/20/26
 * @Description: Component abstract base class and 
 *    all the concrete subclasses (Composite, Circle,
 *    Rectangle, Image, Text, Null). This file uses
 *    Composite + factory patterns.
 **************************************************/

#pragma once

#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <cstdint>
#include <memory>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include <SFML/Graphics.hpp>
#include "KeyFrame.hpp"

using Json = nlohmann::json;

namespace AP {

/*************************************************
 * Component - abstract class
 *   Inherits from sf::Drawable so any Component
 *    can be passed directly to window.draw() or
 *    target.draw()  
 *************************************************/
class Component: public sf::Drawable {
 public:
  // Factory: constructs and returns the correct subclass
  //  Reads shape field from data
  //  and assigns corresponding shape component
  static std::unique_ptr<Component> fromJson(const Json &data);

  // stores the time so currFrame() returns the right frame
  //   Part A: effectively a no-op (first frame is always shown)
  //   Part B: called every render frame to drive animation
  virtual void tween(sf::Time time);

  // virtual destructor: required when deleting via base pointer
  virtual ~Component() = default;

 protected:
  // base constructor
  explicit Component(const Json &data);

  // pure virtual - each subclass implements its own draw
  //  receives states by value & each subclass can modify states.transform
  //  without affecting the siblings
  virtual void draw(sf::RenderTarget &target,
    sf::RenderStates states) const = 0;

  // returns appropriate KeyFrame
  // Part A: returns first keyframe unconditionally
  // Part B: finds the surrounding pair and interpolates
  virtual KeyFrame currFrame() const;

  // return fill color parsed from "color" in JSON - defaults to white
  sf::Color color() const { return _color; }

  sf::Time time;

 private:
  sf::Color _color;                  // fill color for component
  std::vector<KeyFrame> keyframes;   // timeline loaded from JSON
};

/*********************************************
 * NullComponent - silent no-op
**********************************************/
class NullComponent : public Component {
 public:
  explicit NullComponent(const Json& data) : Component(data) {}

 private:
  void draw(sf::RenderTarget&, sf::RenderStates) const override {}
};

/*****************************************************************
* CircleComponent 
*   Draws an sf::CircleShape centered at keyframe's pos
      and origin is shifted to the radius, (x, y) = circle center
*****************************************************************/
class CircleComponent : public Component {
 public:
  explicit CircleComponent(const Json &data);

 protected:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

 private:
  float radius;        // radius in pixels
  std::size_t sides;   // point count - defaults to 30
};

/*************************************************
 * Rectangle Component - draws a rectangle shape
*************************************************/
class RectangleComponent : public Component {
 public:
  explicit RectangleComponent(const Json &data);

 protected:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

 private:
  float x, y;             // local position offset - defaults to 0
  float width, height;    // size in pixels - defaults to 0
};

/***********************************************
 * Image Component - loads a task from disk and
 *  draws it to a sprite with origin set to 
 *  texture center so (x, y) = image center
***********************************************/
class ImageComponent : public Component {
 public:
  explicit ImageComponent(const Json& data);

 protected:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

 private:
  sf::Texture texture;           // loaded once at construction
  std::string file;              // string content to display
  std::string fontFile;          // path kept for error reporting
};

/********************************************************
 * TextComponent - loads a font and draws a text string
********************************************************/
class TextComponent : public Component {
 public:
  explicit TextComponent(const Json &data);

 protected:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

 private:
  sf::Font font;               // loaded once at construction
  std::string text;            // string content to display
  std::string fontFile;        // path kept for error reporting
};

/*****************************************************************
 * CompositeComponent - owns an ordered list of child Components
******************************************************************/
class CompositeComponent : public Component {
 public:
  explicit CompositeComponent(const Json &data);

  // override tween() to propagate time to all children,
  //  otherwise children would never update their time
  void tween(sf::Time time) override;

 protected:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

 private:
  // unique_ptr ownership - children are freed automatically
  // when this composite is destroyed
  std::vector<std::unique_ptr<Component>> children;
};
}    // namespace AP
#endif
