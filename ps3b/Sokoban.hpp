/********************************************************
 * Copyright 2026 Dr. Daly edited by Salma Boukdad
 * @Author: Salma Boukdad
 * @Course: COMP.2040.203
 * @Date: 2/20/26
 * @Description: Sokoban class blueprint - sets up the 
 *   Sokoban game with controls
*********************************************************/

#pragma once

#ifndef SOKOBAN_HPP
#define SOKOBAN_HPP

#include <iostream>
#include <map>
#include <stack>
#include <vector>
#include <string>
#include <utility>
#include "SpriteSheet.hpp"

namespace SB {
// undo/redo stack helper for cleaner interpretation
using gameState = std::pair<std::vector<char>, sf::Vector2u>;

// gameplay directions
enum class Direction {
  Up, Down, Left, Right
};

class Sokoban : public sf::Drawable {
 public:
  static const int TILE_SIZE = 64;

  Sokoban();                              // default constructor
  explicit Sokoban(const std::string&);   // param constructor - string level

  // return dimensions of board game in vector
  sf::Vector2u size() const { return {width(), height()};}
  sf::Vector2u windowSize() const;                        // return window size
  sf::Vector2u playerLoc() const;         // return player position with vector

  unsigned int height() const;                           // return board height
  unsigned int width() const;                             // return board width
  unsigned int gridSize() const;                       // return gridState size
  unsigned int undoSize() const;                       // return undoStack size
  unsigned int redoSize() const;                       // return redoStack size

  char tileAt(unsigned int x, unsigned int y);          // return char at (x,y)
  std::string filename() const;                         // return levelFilename

  bool isWon() const;                // return whether player has won

  void movePlayer(Direction dir);    // take direction to change players loc
  void reset();                      // start level over
  void undo();                       // Optional XC - undo last move
  void redo();                       // Optional XC - redo last move


 protected:
  void draw(sf::RenderTarget &target, sf::RenderStates states)
  const override;


 private:
  std::map<Direction, sf::Vector2u> directionMap;   // store player's direction
  std::map<char, sf::Vector2u> tileMap;     // map cells chars to tSheet coords

  sb::SpriteSheet spriteSheet;              // loads and owns tilesheet texture

  int moveCount = 0;                      // Optional XC - track player's moves
  int boxCounter = 0;                                 // boxes unstore in level

  sf::Font font;

  sf::Vector2u boardDim = {0, 0};                  // width and height of board
  sf::Vector2u playerLocation = {0, 0};     // x and y coord of player location

  std::vector<char> gridState = {};                 // vector of level elements
  std::string levelFilename;                   // fileName for respective level

  Direction playerDirection = Direction::Down;      // set face down by default

  // player's move components require: a vector of the grid elements
  // and the player's desired coords
  std::stack<std::pair<std::vector<char>, sf::Vector2u>> undoStack;
  std::stack<std::pair<std::vector<char>, sf::Vector2u>> redoStack;

  void setMaps();                        // helper function to set our maps

  // input and output operator overload - made friend to access priv members
  friend std::ostream &operator<<(std::ostream &out, const Sokoban &s);
  friend std::istream &operator>>(std::istream &in, Sokoban &s);
};
}  // namespace SB

#endif
