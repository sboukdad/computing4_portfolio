/*******************************************************
 * Copyright Salma Boukdad
 *
 * Filename: EDistance.hpp
 * Course: COMP.2040.203
 * Date: 5/20/26
 * Description: The class skeleton for EDistance, this
 *    calculates the minimum edit distance between two
 *    strings. 
 *******************************************************/

#pragma once
#ifndef EDISTANCE_HPP
#define EDISTANCE_HPP

#include <string>
#include <vector>

class EDistance {
 public:
  EDistance(const std::string& s1, const std::string& s2);

  static int penalty(char a, char b);
  static int min3(int a, int b, int c);

  int optDistance();         // fills the matrix
  std::string alignment();   // sums up the costs
 private:
  std::string x, y;                          // x: stringX, y: stringY
  int M, N;                                  // M: size of x, N: size of y
  std::vector<std::vector<int>> opt;         // opt: holds the matrix of costs
};
#endif
