// Copyright Salma Boukdad & Jeannine Al Attal 2026

#include <fstream>
#include <stdexcept>
#include <string>
#include <nlohmann/json.hpp>
#include "AniPlayer.hpp"

using Json = nlohmann::json;

namespace AP {
// param constructor: opens and parses the JSON file
AniPlayer::AniPlayer(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::invalid_argument("Could not open file: " + filename);
    }

    Json data = Json::parse(file);

    // read window dimensions
    width = data["width"].get<unsigned int>();
    height = data["height"].get<unsigned int>();

    // builds entire scene tree from JSON "scene" object
    scene = Component::fromJson(data["scene"]);
}

// partB: advances animation time across full component tree
void AniPlayer::tween(sf::Time time) {
    if (scene) {
        scene->tween(time);
    }
}

// return (width, height) so main() can size the window correctly
sf::Vector2u AniPlayer::windowSize() const {
    return {width, height};
}

// called by window.draw(player) via sf::Drawable interface
// passes identity RenderStates with no parent transform existing at
// this level. Transforms accumulate as the call descends the tree
void AniPlayer::draw(sf::RenderTarget &target, sf::RenderStates states)
const {
    if (scene) {
        target.draw(*scene, states);
    }
}
}   // namespace AP

