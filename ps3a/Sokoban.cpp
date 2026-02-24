/***************************************************************************
 * Copyright 2026 Salma Boukdad
 * @Author: Salma Boukdad
 * @Course: COMP.2040.203
 * @Date: 2/20/26
 * @Description: Implementation for the Sokoban header file,
 *  including functions such as redo, an overriden draw, and playerLocation
 ***************************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include "Sokoban.hpp"

namespace SB {
// default constructor
Sokoban::Sokoban() : spriteSheet("sokoban_tilesheet.png",
  {TILE_SIZE, TILE_SIZE}, 0) {
  setMaps();
}

// param constructor that takes level filename
Sokoban::Sokoban(const std::string& level) :
  spriteSheet("sokoban_tilesheet.png", {TILE_SIZE, TILE_SIZE}, 0) {
  setMaps();
  levelFilename = level;
  std::ifstream inFile(level);

  if (!inFile.is_open()) {
    std::cerr << "Error: unable to open file: " << level << std::endl;
    return;
  }
  inFile >> *this;
}

// return height of board
unsigned int Sokoban::height() const {
  return boardDim.y;
}

// return width of board
unsigned int Sokoban::width() const {
  return boardDim.x;
}

// return dimensions of window
sf::Vector2u Sokoban::windowSize() const {
  sf::Vector2u windowDim;
  windowDim.x = boardDim.x * spriteSheet.getTileSize().x;
  windowDim.y = boardDim.y * spriteSheet.getTileSize().y;
  return windowDim;
}

// return coordinates of player's current location
sf::Vector2u Sokoban::playerLoc() const {
  return playerLocation;
}

// check if the player has won
bool Sokoban::isWon() const {
  return false;
}

// move the player on the board
void Sokoban::movePlayer(Direction) {}    // updates stacks in part b

// reset board to original setup
void Sokoban::reset() {
  if (levelFilename.empty()) {
    return;
  }
  std::ifstream inFile(levelFilename);
  if (!inFile.is_open()) {
    std::cerr << "Error: unable to open file: " << levelFilename << std::endl;
  }
  inFile >> *this;
  moveCount = 0;
}

// undo last move
void Sokoban::undo() {
  if (undoStack.empty()) {
    return;
  }
  gameState lastMove = undoStack.top();
  undoStack.pop();
  gridState = lastMove.first;
  playerLocation = lastMove.second;
  redoStack.push({gridState, playerLocation});
}

// redo last move
void Sokoban::redo() {
  if (redoStack.empty()) {
    return;
  }
  gameState lastMove = redoStack.top();
  redoStack.pop();
  gridState = lastMove.first;
  playerLocation = lastMove.second;
  undoStack.push({gridState, playerLocation});
}

// helper function to set Sokoban maps and reduce duplicate code
void Sokoban::setMaps() {
  tileMap['#'] = {8, 6};        // wall
  tileMap['.'] = {12, 6};       // empty floor
  tileMap['a'] = {12, 7};       // storage location
  tileMap['A'] = {7, 0};        // box
  tileMap['1'] = {9, 0};        // box on storage
  tileMap['@'] = {0, 4};        // player (facing down default)

  directionMap[Direction::Up] = {3, 4};
  directionMap[Direction::Down] = {0, 4};
  directionMap[Direction::Left] = {4, 6};
  directionMap[Direction::Right] = {1, 6};
}

void Sokoban::draw(sf::RenderTarget& target, sf::RenderStates) const {
  unsigned int width, height;
  width = boardDim.x;
  height = boardDim.y;

  // loop through all elements
  for (unsigned int j = 0; j < height; j++) {
    for (unsigned int i = 0; i < width; i++) {
      char c = gridState[i + j * width];      // get board element

      // skip if char is not in tileMap
      if (tileMap.count(c) == 0) {
        continue;
      }

      sf::Sprite sprite = spriteSheet[tileMap.at(c)].toSprite();
      sprite.setPosition({static_cast<float>(i * TILE_SIZE),
        static_cast<float>(j * TILE_SIZE)});
      target.draw(sprite);
    }
  }

  // draw player
  sf::Sprite player = spriteSheet[directionMap.at(playerDirection)].toSprite();
  player.setPosition({static_cast<float>(playerLocation.x * TILE_SIZE),
                      static_cast<float>(playerLocation.y * TILE_SIZE)});
  target.draw(player);
}

// input operator overload - read in board dim then its board layout
std::istream &operator>>(std::istream& in, Sokoban& s) {
  // read in (col, rows)
  unsigned int width, height;
  in >> height >> width;
  s.boardDim.x = width;
  s.boardDim.y = height;

  // resize gridState
  s.gridState.resize(width * height);

  // loop through grid
  for (unsigned int j = 0; j < height; j++) {
    for (unsigned int i = 0; i < width; i++) {
      char c;
      in >> c;

      // check if @ found (<- character syboml)
      if (c == '@') {
        // store player location then replace @ with floor
        s.playerLocation.x = i;
        s.playerLocation.y = j;
        c = '.';
      }
      s.gridState[i + j * width] = c;
    }
  }
  return in;
}

// output operator overload - display dim then board layout w/ chars
std::ostream &operator<<(std::ostream &out, const Sokoban &s) {
  // dimensions we must print
  unsigned int width, height;
  width = s.boardDim.x;
  height = s.boardDim.y;
  out << height << " " << width << std::endl;

  // nested loop to traverse grid
  for (unsigned int j = 0; j < height; j++) {
    for (unsigned int i = 0; i < width; i++) {
      char c = s.gridState[i + j * width];

      // if (i, j) == player location
      if (i == s.playerLocation.x && j == s.playerLocation.y) {
        c = '@';
      }
      out << c;
    }
    out << std::endl;         // print new line for each new row
  }
  return out;
}
}       // namespace SB
