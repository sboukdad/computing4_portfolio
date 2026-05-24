// Copyright Salma Boukdad & Jeannine Al Attal 2026

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <algorithm>
#include "Component.hpp"


namespace AP {

/*****************************************************
 * Component - base class
******************************************************/
// base constructor - every subclass calls this via
Component::Component(const Json& data) {
    // map of color options
    static const std::unordered_map<std::string, std::string> colorNames = {
        {"white",   "FFFFFFFF"}, {"silver",  "C0C0C0FF"},
        {"gray",    "808080FF"}, {"black",   "000000FF"},
        {"red",     "FF0000FF"}, {"maroon",  "800000FF"},
        {"yellow",  "FFFF00FF"}, {"olive",   "808000FF"},
        {"lime",    "00FF00FF"}, {"green",   "008000FF"},
        {"aqua",    "00FFFFFF"}, {"teal",    "008080FF"},
        {"blue",    "0000FFFF"}, {"navy",    "000080FF"},
        {"fuchsia", "FF00FFFF"}, {"purple",  "800080FF"}
    };

    // color in a hex string, defaults to white
    std::string hex = data.value("color", "FFFFFFFF");

    // convert to lowercase for case sensitive matching
    std::string lower = hex;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    // check if it's a named color then replace with hex equivalent
    if (colorNames.count(lower)) {
        hex = colorNames.at(lower);
    }

    // padded to 8 chars so the bit-shift math is always correct
    if (hex.size() == 6) {
        hex += "FF";
    }

    // parse full 32-bit RGBA value, then split into bytes
    uint64_t rgba = std::stoul(hex, nullptr, 16);
    _color = sf::Color(
        static_cast<std::uint8_t>((rgba >> 24) & 0xFF),     // red
        static_cast<std::uint8_t>((rgba >> 16) & 0xFF),     // green
        static_cast<std::uint8_t>((rgba >> 8) & 0xFF),      // blue
        static_cast<std::uint8_t>(rgba & 0xFF));            // alpha

    // keyframes
    if (data.contains("keyframes")) {
        for (const Json& kf : data["keyframes"]) {
            keyframes.emplace_back(kf);         // KeyFrame(Json) constructor
        }
    }
}

// stores the playback time for currFrame() to use
//  part A never calls this
//  in part B, aniplayer calls this every frame with the clock time
void Component::tween(sf::Time time) {
    this->time = time;
}

// return the KeyFrame that applies at time
//  in part A time is always zero and returns first keyFrame
//  in part B time advances with the clock
KeyFrame Component::currFrame() const {
    // no keyframes
    if (keyframes.empty()) {
        return KeyFrame{};
    }

    // before or at first frame, locks to start
    if (keyframes.size() == 1 || this->time <= keyframes.front().time()) {
        return keyframes.front();
    }

    // past the last frame, locks to end (animation is done)
    if (time >= keyframes.back().time()) {
        return keyframes.back();
    }

    for (std::size_t i = 0; i + 1 < keyframes.size(); ++i)  {
        const KeyFrame &a = keyframes[i];
        const KeyFrame &b = keyframes[i + 1];
        if (this->time >= a.time() && this->time <= b.time()) {
            return a.tween(b, this->time);
        }
    }

    // fallback, is never reached with a valid keyframe list
    return keyframes.front();
}

// FACTORY
//  this maps a shape string to a subclass - both AniPlayer
//  and CompositeComponent both call this without needing to know
//  which types exist
std::unique_ptr<Component> Component::fromJson(const Json& data) {
    const Json &payload = data.contains("scene") ? data["scene"] : data;

    if (!payload.contains("shape")) {
        throw std::invalid_argument("Missing \"shape\" field in component.");
    }

    std::string shape = payload["shape"].get<std::string>();

    if (shape == "circle") {
        return std::make_unique<CircleComponent>(payload);
    } else if (shape == "rect") {
        return std::make_unique<RectangleComponent>(payload);
    } else if (shape == "image") {
        return std::make_unique<ImageComponent>(payload);
    } else if (shape == "text") {
        return std::make_unique<TextComponent>(payload);
    } else if (shape == "composite") {
        return std::make_unique<CompositeComponent>(payload);
    } else {
        throw std::invalid_argument("Unknown component shape: \"" +
        shape + "\".");
    }
}

/**********************************************************
 * CircleComponent
***********************************************************/
CircleComponent::CircleComponent(const Json& data) : Component(data) {
    if (!data.contains("radius")) {
        throw std::invalid_argument("CircleComponent missing \"radius\".");
    }

    radius = data.value("radius", 0.0f);

    // sides defaults to 30, matching circleShape's default point count
    sides = data.value("sides", static_cast<std::size_t>(30));
}

void CircleComponent::draw(sf::RenderTarget& target,
 sf::RenderStates states) const {
    // step 1: get the frame for the current time
    KeyFrame kf = currFrame();

    // step 2: bake translation/rotation/scale into the local states copy
    //  states are passed by value, so this does not affect sibling components
    kf.transform(states.transform);

    // step 3: build and draw the shape
    sf::CircleShape shape(radius, sides);

    // shift origin to center: keyframe (x, y) = center of circle (per spec)
    shape.setOrigin({radius, radius});
    shape.setFillColor(color());

    target.draw(shape, states);
}

/**********************************************
* RectangleComponent
**********************************************/
RectangleComponent::RectangleComponent(const Json& data) : Component(data) {
    // all fields default to 0 (making the rectangle invisible)
    x = data.value("x", 0.0f);
    y = data.value("y", 0.0f);
    width = data.value("width", 0.0f);
    height = data.value("height", 0.0f);
}

void RectangleComponent::draw(sf::RenderTarget& target,
 sf::RenderStates states) const {
    // step 1 + 2: get frame and apply transform
    KeyFrame kf = currFrame();
    kf.transform(states.transform);

    // step 3: build and draw the shape
    sf::RectangleShape shape({width, height});
    shape.setPosition({x, y});
    shape.setFillColor(color());

    target.draw(shape, states);
}

/************************************************
* ImageComponent
************************************************/
ImageComponent::ImageComponent(const Json& data) : Component(data) {
    if (!data.contains("file")) {
        throw std::invalid_argument("ImageComponent missing \"file\".");
    }

    file = data["file"].get<std::string>();

    // load texture at construction time, not inside draw()
    if (!texture.loadFromFile(file)) {
        throw std::runtime_error("Could not load image:" + file);
    }
}

void ImageComponent::draw(sf::RenderTarget& target,
     sf::RenderStates states) const {
        // step 1 + 2: get frame and apply transform
        KeyFrame kf = currFrame();
        kf.transform(states.transform);

        // step 3: build sprite from pre-loaded member texture
        sf::Sprite sprite(texture);

        // center origin so keyframe (x,y) = image center (per spec)
        sf::Vector2u sz = texture.getSize();
        sprite.setOrigin({sz.x / 2.0f, sz.y / 2.0f});

        // no setFillColor, sprite colors come from the texture pixels
        target.draw(sprite, states);
}

/**************************************************
* TextComponent
**************************************************/
TextComponent::TextComponent(const Json& data) : Component(data) {
    if (!data.contains("font")) {
        throw std::invalid_argument("TextComponent missing \"font\".");
    }

    fontFile = data["font"].get<std::string>();
    text = data.value("text", "");             // empty string if "text" absent

    // load font at construction time, same reasoning as ImageComponent
    if (!font.openFromFile(fontFile)) {
        throw std::runtime_error("Could not load font: " + fontFile);
    }
}

void TextComponent::draw(sf::RenderTarget& target,
 sf::RenderStates states) const {
    // step 1 + 2: get frame and apply transform
    KeyFrame kf = currFrame();
    kf.transform(states.transform);

    // step 3: build and draw the text
    sf::Text text(font);
    text.setString(this->text);
    text.setFillColor(color());

    // per spec text draws from its upper-left corner
    target.draw(text, states);
}

/***********************************************
 * CompositeComponent
***********************************************/
CompositeComponent::CompositeComponent(const Json& data) : Component(data) {
    // recursively construct each child via fromJson()
    if (data.contains("children")) {
        for (const Json& child : data["children"]) {
            children.push_back(Component::fromJson(child));
        }
    }
}

// propagates time to all children so they animate correctly, the
// base class tween() updates time for this composite's own currFrame()
// then forwards the same time to every child
void CompositeComponent::tween(sf::Time time) {
    Component::tween(time);         // updates its own time
    for (auto& child : children) {
        child->tween(time);         // forward to each child
    }
}

void CompositeComponent::draw(sf::RenderTarget& target,
 sf::RenderStates states) const {
    // applies this composite's own keyframe transform to states,
    // states is a local value copy, so this doesn't affect the caller
    KeyFrame kf = currFrame();
    kf.transform(states.transform);

    // draw every child, passing the offset states
    // each child calls kf.tranform() again on its own copy of states
    for (const auto& child : children) {
        target.draw(*child, states);
    }
}
}   // namespace AP
