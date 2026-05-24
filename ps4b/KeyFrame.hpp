/***************************************************
 * Copyright Salma Boukdad & Jeannine Al Attal
 * @Authors: Salma Boukdad, Jeannine Al Attal
 * @Course: COMP.2040.203
 * @Date: 3/20/26
 * @Description:
 **************************************************/

#pragma once

#ifndef KEYFRAME_HPP
#define KEYFRAME_HPP

#include <algorithm>
#include <nlohmann/json.hpp>
#include <SFML/Graphics.hpp>

using Json = nlohmann::json;

namespace AP {
class KeyFrame {
 private:
  // attributes (variables) go here

  // t: time in seconds frame applies, x + y: respective translation in pixels
  float t, x, y;
  // scaleX: horizontal scale, scaleY: vertical scale, theta: rot. in degrees
  float scaleX, scaleY, theta;

  // clamping to prevent extrapolation past keyframe endpoints
  // clamps i to [0, 1] then returns a + i*(b - a)
  static float lerp(float a, float b, float i);

 public:
  // methods (functions) go here

  /**
   * Default constructs this KeyFrame
   */
  KeyFrame();

  /**
   * Constructs this KeyFrame by extracting data from a Json object
   * @param json A Json object containing KeyFrame data
   */
  explicit KeyFrame(const Json &json);

  /**
   * Gets the time at this KeyFrame
   */
  sf::Time time() const;

  /**
   * Computes the frame between this frame and the other one using linear interpolation
   * @param alt Another KeyFrame
   * @param t The time between the two KeyFrames to tween to
   */
  KeyFrame tween(KeyFrame alt, sf::Time t) const;   // For part B

  /**
   * Applies the transform information in this keyframe to the given Transform.
   * @param state A Transform object that will be modified
   */
  void transform(sf::Transform &state) const;

  // getters used by tween() to read values from the alternate frame
  float getT() const { return t; }
  float getX() const { return x; }
  float getY() const { return y; }
  float getScaleX() const { return scaleX; }
  float getScaleY() const { return scaleY; }
  float getTheta() const { return theta; }
  void setX(float v) { x = v; }
  void setY(float v) { y = v; }
  void setScaleX(float v) { scaleX = v; }
  void setScaleY(float v) { scaleY = v; }
  void setTheta(float v) { theta = v; }
};
}  // namespace AP
#endif
