// Copyright Salma Boukdad & Jeannine Al Attal

#include <algorithm>
#include <stdexcept>
#include "KeyFrame.hpp"

namespace AP {
// default constructor (identity frame)
KeyFrame::KeyFrame() : t(0.0f), x(0.0f), y(0.0f),
scaleX(1.0f), scaleY(1.0f), theta(1.0f) {}

// JSON constructor reads each field from the obj
KeyFrame::KeyFrame(const Json& json) {
    if (!json.contains("t")) {
        throw std::invalid_argument("\"t\" field missing from keyframe.");
    }

    t = json["t"].get<float>();             // default values below
    x = json.value("x", 0.0f);              // no x offset
    y = json.value("y", 0.0f);              // no y offset
    scaleX = json.value("scale-x", 1.0f);   // 1:1 x scale
    scaleY = json.value("scale-y", 1.0f);   // 1:1 y scale
    theta = json.value("theta", 0.0f);      // no rotation
}

// wraps t as sf::Time so callers can compare against sf::Clock values
sf::Time KeyFrame::time() const {
    return sf::seconds(t);
}

// linear interpolation between this frame (start) and alt (end)
// at the requested time t
KeyFrame KeyFrame::tween(KeyFrame alt, sf::Time t) const {
    // part B
    float range = alt.getT() - this->t;

    // check for division by zero, if both frames share a timestamp
    // lock to the start frame (i = 0.0)
    float i = (range != 0.0f) ? (t.asSeconds() - this->t) / range : 0.0f;

    // create an intermediate Json so it can reuse the Json constructor
    Json kf = {{"t", t.asSeconds()}, {"x", lerp(x, alt.getX(), i)},
    {"y", lerp(y, alt.getY(), i)},
    {"scale-x", lerp(scaleX, alt.getScaleX(), i)},
    {"scale-y", lerp(scaleY, alt.getScaleY(), i)},
    {"theta", lerp(theta, alt.getTheta(), i)}};

    return KeyFrame(kf);
}

// applies this frame's position/rotation/scale to an sf::Transform
// process: translate -> rotate -> scale
void KeyFrame::transform(sf::Transform& state) const {
    state.translate({x, y});
    state.rotate(sf::degrees(theta));
    state.scale({scaleX, scaleY});
}

// clamps i to [0, 1] then blends a towards b
float KeyFrame::lerp(float a, float b, float i) {
    i = std::max(0.0f, std::min(1.0f, i));
    return a + i * (b - a);
}
}   // namespace AP
