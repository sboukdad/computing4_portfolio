// Copyright Salma Boukdad & Jeannine Al Attal 2026

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>
#include "Component.hpp"


namespace AP {

/*****************************************************
 * Component - base class
******************************************************/
// base constructor - every subclass calls this via their initializer list
Component::Component(const Json& data) {
    // color in a hex string, defaults to white
    std::string hex = data.value("color", "FFFFFFFF");

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

    // keyframes — load each one from the JSON array
    // extra credit: carry over missing fields from the previous keyframe
    // so that only changed values need to be specified in the JSON
    if (data.contains("keyframes")) {
        for (const Json& kf : data["keyframes"]) {
            KeyFrame frame(kf);
            // if there is a previous keyframe, inherit any missing fields
            if (!keyframes.empty()) {
                const KeyFrame& prev = keyframes.back();
                if (!kf.contains("x"))       frame.setX(prev.getX());
                if (!kf.contains("y"))       frame.setY(prev.getY());
                if (!kf.contains("scale-x")) frame.setScaleX(prev.getScaleX());
                if (!kf.contains("scale-y")) frame.setScaleY(prev.getScaleY());
                if (!kf.contains("theta"))   frame.setTheta(prev.getTheta());
            }
            keyframes.emplace_back(frame);
        }
    }
}

// stores the playback time for currFrame() to use
//  part A never calls this
//  in part B, AniPlayer calls this every frame with the clock time
void Component::tween(sf::Time time) {
    this->time = time;
}

// return the KeyFrame that applies at the current time
//  in part A time is always zero, so first keyFrame is always returned
//  in part B time advances with the clock and interpolation kicks in
KeyFrame Component::currFrame() const {
    // no keyframes — return identity frame
    if (keyframes.empty()) {
        return KeyFrame{};
    }

    // before or at first frame — lock to start
    if (keyframes.size() == 1 || this->time <= keyframes.front().time()) {
        return keyframes.front();
    }

    // past the last frame — lock to end so animation holds its final position
    if (time >= keyframes.back().time()) {
        return keyframes.back();
    }

    // find the two keyframes that bracket the current time and interpolate
    for (std::size_t i = 0; i + 1 < keyframes.size(); ++i)  {
        const KeyFrame &a = keyframes[i];
        const KeyFrame &b = keyframes[i + 1];
        if (this->time >= a.time() && this->time <= b.time()) {
            return a.tween(b, this->time);
        }
    }

    // fallback — never reached with a valid keyframe list
    return keyframes.front();
}

// FACTORY
//  maps a shape string to the correct subclass — both AniPlayer
//  and CompositeComponent call this without needing to know which types exist
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

    radius = data["radius"].get<float>();

    // sides defaults to 30, matching CircleShape's default point count
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
    // all fields default to 0 (making the rectangle invisible if unset)
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
    // this prevents performance lag during the render loop
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

        // no setFillColor — sprite colors come from the texture pixels
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
    // per spec, text draws from its upper-left corner
    sf::Text text(font);
    text.setString(this->text);
    text.setFillColor(color());

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

// propagates time to all children so they animate correctly
// the base class tween() updates time for this composite's own currFrame()
// then forwards the same time to every child
void CompositeComponent::tween(sf::Time time) {
    Component::tween(time);         // updates its own time
    for (auto& child : children) {
        child->tween(time);         // forward to each child
    }
}

void CompositeComponent::draw(sf::RenderTarget& target,
 sf::RenderStates states) const {
    // applies this composite's own keyframe transform to states
    // states is a local value copy, so this doesn't affect the caller
    KeyFrame kf = currFrame();
    kf.transform(states.transform);

    // draw every child, passing the modified states
    // each child calls kf.transform() again on its own copy of states
    for (const auto& child : children) {
        target.draw(*child, states);
    }
}
}   // namespace AP
