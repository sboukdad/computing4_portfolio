// Copyright 2026 Salma Boukdad
/*************************
 * @Author: Salma Boukdad
 * @Course: COMP4.2040.203
 * @Date: 2/14/26
 *
 *************************/

#ifndef PTREE_HPP
#define PTREE_HPP

#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>


class PTree : public sf::Drawable {
 private:
    std::vector<sf::RectangleShape> squares;        // store all squares
    float baseSize;
    int depth;
    int maxDepth;

    // helper function that will do the actual recursion
    void buildTree(
        float x, float y, float size, float rotation, int depth);

    // override draw
    void draw(sf::RenderTarget &target, sf::RenderStates states)
        const override;

 public:
    PTree(float L, int N);
};
#endif
