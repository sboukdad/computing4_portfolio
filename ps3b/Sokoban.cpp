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
#include <algorithm>
#include <string>
#include <SFML/Graphics.hpp>
#include "Sokoban.hpp"

namespace SB {
// default constructor
Sokoban::Sokoban() : spriteSheet("sokoban_tilesheet.png",
  {TILE_SIZE, TILE_SIZE}, 0) {
  setMaps();
  font = sf::Font("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
}

// param constructor that takes level filename
Sokoban::Sokoban(const std::string& level) :
  spriteSheet("sokoban_tilesheet.png", {TILE_SIZE, TILE_SIZE}, 0) {
  setMaps();
  levelFilename = level;
  font = sf::Font("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
  std::ifstream inFile(level);

  if (!inFile.is_open()) {
    std::cerr << "Error: unable to open file: " << level << std::endl;
    return;
  }
  inFile >> *this;
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

// return height of board
unsigned int Sokoban::height() const {
  return boardDim.y;
}

// return width of board
unsigned int Sokoban::width() const {
  return boardDim.x;
}

// return size of GridState
unsigned int Sokoban::gridSize() const {
  return gridState.size();
}

// return size of undoStack
unsigned int Sokoban::undoSize() const {
  return undoStack.size();
}

// return size of redoStack
unsigned int Sokoban::redoSize() const {
  return redoStack.size();
}

// return char at position (x, y)
char Sokoban::tileAt(unsigned int x, unsigned int y) {
  char c = gridState[x + y * width()];
  return c;
}

// return levelFilename
std::string Sokoban::filename() const {
  return levelFilename;
}

// check if game is won
bool Sokoban::isWon() const {
  return std::count_if(gridState.begin(), gridState.end(), [](char c) {
    return c == 'A';
  }) == 0;
}

// move the player on the board
void Sokoban::movePlayer(Direction dir) {
  auto currentState = gridState;
  auto currentLocation = playerLocation;

  switch (dir) {
    // get current board for undo
    case Direction::Up:
      // if move exceeds board dim
      if ((playerLocation.y) == 0) return;
      // if tile is a wall
      if (tileAt(playerLocation.x, playerLocation.y - 1) == '#') return;

      // box validity (check for double box / walls)
      if (tileAt(playerLocation.x, playerLocation.y - 1) == 'A' ||
          tileAt(playerLocation.x, playerLocation.y - 1) == '1') {
        if (tileAt(playerLocation.x, playerLocation.y - 2) == 'A') return;
        if (tileAt(playerLocation.x, playerLocation.y - 2) == '#') return;
        if (tileAt(playerLocation.x, playerLocation.y - 2) == '1') return;
      }

      // restore current tiles
      if (tileAt(playerLocation.x, playerLocation.y) == 'p') {
        gridState[playerLocation.x + playerLocation.y * width()] = 'a';
      } else {
        gridState[playerLocation.x + playerLocation.y * width()] = '.';
      }

      // move box
      if (tileAt(playerLocation.x, playerLocation.y - 1) == 'A' ||
          tileAt(playerLocation.x, playerLocation.y - 1) == '1') {
        if (tileAt(playerLocation.x, playerLocation.y - 2) == '.') {
          gridState[playerLocation.x + (playerLocation.y - 2) * width()] = 'A';
        } else if (tileAt(playerLocation.x, playerLocation.y - 2) == 'a') {
          gridState[playerLocation.x + (playerLocation.y - 2) * width()] = '1';
        }

        if (tileAt(playerLocation.x, playerLocation.y - 1) == '1') {
          gridState[playerLocation.x + (playerLocation.y - 1) * width()] = 'a';
        } else {
          gridState[playerLocation.x + (playerLocation.y - 1) * width()] = '.';
        }
      }

      // player on storage tile
      if (tileAt(playerLocation.x, playerLocation.y - 1) == 'a') {
        gridState[playerLocation.x + (playerLocation.y - 1) * width()] = 'p';
      }

      // update direction and location
      playerDirection = Direction::Up;
      playerLocation.y -= 1;
      break;

    case Direction::Down:
      // no move beyond board dim
      if ((playerLocation.y) == height() - 1) return;
      // block wall move
      if (tileAt(playerLocation.x, playerLocation.y + 1) == '#') return;

      // block box and wall blocks
      if (tileAt(playerLocation.x, playerLocation.y + 1) == 'A' ||
          tileAt(playerLocation.x, playerLocation.y + 1) == '1') {
        if (tileAt(playerLocation.x, playerLocation.y + 2) == 'A') return;
        if (tileAt(playerLocation.x, playerLocation.y + 2) == '#') return;
        if (tileAt(playerLocation.x, playerLocation.y + 2) == '1') return;
      }

      // restore storage locations
      if (tileAt(playerLocation.x, playerLocation.y) == 'p') {
        gridState[playerLocation.x + playerLocation.y * width()] = 'a';
      } else {
        gridState[playerLocation.x + playerLocation.y * width()] = '.';
      }

      // box interactions
      if (tileAt(playerLocation.x, playerLocation.y + 1) == 'A' ||
          tileAt(playerLocation.x, playerLocation.y + 1) == '1') {
        if (tileAt(playerLocation.x, playerLocation.y + 2) == '.') {
          gridState[playerLocation.x + (playerLocation.y + 2) * width()] = 'A';
        } else if (tileAt(playerLocation.x, playerLocation.y + 2) == 'a') {
          gridState[playerLocation.x + (playerLocation.y + 2) * width()] = '1';
        }

        if (tileAt(playerLocation.x, playerLocation.y + 1) == '1') {
          gridState[playerLocation.x + (playerLocation.y + 1) * width()] = 'a';
        } else {
          gridState[playerLocation.x + (playerLocation.y + 1) * width()] = '.';
        }
      }

      // player on storage tile
      if (tileAt(playerLocation.x, playerLocation.y + 1) == 'a') {
        gridState[playerLocation.x + (playerLocation.y + 1) * width()] = 'p';
      }

      // update playerLocation
      playerDirection = Direction::Down;
      playerLocation.y += 1;
      break;

    case Direction::Left:
      // no move beyond board dimensions
      if ((playerLocation.x) == 0) return;
      // block wall move
      if (tileAt(playerLocation.x - 1, playerLocation.y) == '#') return;

      // block box and wall blocks
      if (tileAt(playerLocation.x - 1, playerLocation.y) == 'A' ||
          tileAt(playerLocation.x - 1, playerLocation.y) == '1') {
        if (tileAt(playerLocation.x - 2, playerLocation.y) == 'A') return;
        if (tileAt(playerLocation.x - 2, playerLocation.y) == '#') return;
        if (tileAt(playerLocation.x - 2, playerLocation.y) == '1') return;
      }

      // restore storage locations
      if (tileAt(playerLocation.x, playerLocation.y) == 'p') {
        gridState[playerLocation.x + playerLocation.y * width()] = 'a';
      } else {
        gridState[playerLocation.x + playerLocation.y * width()] = '.';
      }

      // box interactions
      if (tileAt(playerLocation.x - 1, playerLocation.y) == 'A' ||
          tileAt(playerLocation.x - 1, playerLocation.y) == '1') {
        if (tileAt(playerLocation.x - 2, playerLocation.y) == '.') {
          gridState[(playerLocation.x - 2) + playerLocation.y * width()] = 'A';
        } else if (tileAt(playerLocation.x - 2, playerLocation.y) == 'a') {
          gridState[(playerLocation.x - 2) + playerLocation.y * width()] = '1';
        }

        if (tileAt(playerLocation.x - 1, playerLocation.y) == '1') {
          gridState[(playerLocation.x - 1) + playerLocation.y * width()] = 'a';
        } else {
          gridState[(playerLocation.x - 1) + playerLocation.y * width()] = '.';
        }
      }

      // player on storage tile
      if (tileAt(playerLocation.x - 1, playerLocation.y) == 'a') {
        gridState[(playerLocation.x - 1) + playerLocation.y * width()] = 'p';
      }

      // update playerLocation
      playerDirection = Direction::Left;
      playerLocation.x -= 1;
      break;

    case Direction::Right:
      // no move beyond board dim
      if ((playerLocation.x) == width() - 1) return;
      // block wall movements
      if (tileAt(playerLocation.x + 1, playerLocation.y) == '#') return;

      // block box and wall blocked wall
      if (tileAt(playerLocation.x + 1, playerLocation.y) == 'A' ||
          tileAt(playerLocation.x + 1, playerLocation.y) == '1') {
        if (tileAt(playerLocation.x + 2, playerLocation.y) == 'A') return;
        if (tileAt(playerLocation.x + 2, playerLocation.y) == '#') return;
        if (tileAt(playerLocation.x + 2, playerLocation.y) == '1') return;
      }

      // restore storage locations
      if (tileAt(playerLocation.x, playerLocation.y) == 'p') {
        gridState[playerLocation.x + playerLocation.y * width()] = 'a';
      } else {
        gridState[playerLocation.x + playerLocation.y * width()] = '.';
      }

      // box interactions
      if (tileAt(playerLocation.x + 1, playerLocation.y) == 'A' ||
          tileAt(playerLocation.x + 1, playerLocation.y) == '1') {
        if (tileAt(playerLocation.x + 2, playerLocation.y) == '.') {
          gridState[(playerLocation.x + 2) + playerLocation.y * width()] = 'A';
        } else if (tileAt(playerLocation.x + 2, playerLocation.y) == 'a') {
          gridState[(playerLocation.x + 2) + playerLocation.y * width()] = '1';
        }

        if (tileAt(playerLocation.x + 1, playerLocation.y) == '1') {
          gridState[(playerLocation.x + 1) + playerLocation.y * width()] = 'a';
        } else {
          gridState[(playerLocation.x + 1) + playerLocation.y * width()] = '.';
        }
      }

      // player on storage tile
      if (tileAt(playerLocation.x + 1, playerLocation.y) == 'a') {
        gridState[(playerLocation.x + 1) + playerLocation.y * width()] = 'p';
      }

      // update playerLocation
      playerDirection = Direction::Right;
      playerLocation.x += 1;
      break;
  }

  // add new board to redo
  undoStack.push({currentState, currentLocation});
  while (!redoStack.empty()) redoStack.pop();

  moveCount++;
}

// reset board to original setup
void Sokoban::reset() {
  if (levelFilename.empty()) return;

  std::ifstream inFile(levelFilename);
  if (!inFile.is_open()) {
    std::cerr << "Error: unable to open file: " << levelFilename << std::endl;
  }
  inFile >> *this;

  while (!undoStack.empty()) undoStack.pop();
  while (!redoStack.empty()) redoStack.pop();

  playerDirection = Direction::Down;
  moveCount = 0;
}

// undo last move
void Sokoban::undo() {
  if (undoStack.empty()) return;

  gameState lastMove = undoStack.top();
  undoStack.pop();
  redoStack.push({gridState, playerLocation});
  gridState = lastMove.first;
  playerLocation = lastMove.second;
  moveCount--;
}

// redo last move
void Sokoban::redo() {
  if (redoStack.empty()) return;

  gameState lastMove = redoStack.top();
  redoStack.pop();
  undoStack.push({gridState, playerLocation});
  gridState = lastMove.first;
  playerLocation = lastMove.second;
  moveCount++;
}

// helper function to set Sokoban maps and reduce duplicate code
void Sokoban::setMaps() {
  tileMap['#'] = {8, 6};        // wall
  tileMap['.'] = {12, 6};       // empty floor
  tileMap['a'] = {12, 7};       // storage location
  tileMap['A'] = {7, 0};        // box
  tileMap['1'] = {9, 0};        // box on storage
  tileMap['@'] = {0, 4};        // player (facing down default)
  tileMap['p'] = {12, 7};       // player on storage location

  directionMap[Direction::Up] = {3, 4};
  directionMap[Direction::Down] = {0, 4};
  directionMap[Direction::Left] = {4, 6};
  directionMap[Direction::Right] = {1, 6};
}

// draw overriden to render Sokoban
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

  // draw player to board
  sf::Sprite player = spriteSheet[directionMap.at(playerDirection)].toSprite();
  player.setPosition({static_cast<float>(playerLocation.x * TILE_SIZE),
                      static_cast<float>(playerLocation.y * TILE_SIZE)});
  target.draw(player);

  // display moveCounter
  sf::Text moveTracker(font, std::to_string(moveCount));
  moveTracker.setCharacterSize(36);
  moveTracker.setFillColor(sf::Color::White);
  moveTracker.setPosition({5.f, 5.f});
  target.draw(moveTracker);

  // display won message
  if (isWon() == true) {
    sf::RectangleShape winMsg({200.f, 80.f});
    winMsg.setFillColor(sf::Color::White);
    winMsg.setPosition({windowSize().x / 2.f - 100.f,
                        windowSize().y / 2.f - 40.f});
    sf::Text text(font, "You win!");
    text.setCharacterSize(30);
    text.setFillColor(sf::Color::Green);
    text.setPosition({
      winMsg.getPosition().x + winMsg.getSize().x / 2.f -
        text.getLocalBounds().size.x / 2.f,
      winMsg.getPosition().y + winMsg.getSize().y / 2.f -
        text.getLocalBounds().size.y / 2.f - 10.f
  });
    target.draw(winMsg);
    target.draw(text);
  }
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
