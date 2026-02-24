// Copyright 2026 Salma Boukdad
/***********************************
 * @Author: Salma Boukdad
 * @Course: COMP.2040.202
 * @Date: 2/17/26
 * @Description: 
************************************/

#include <cmath>
#include <SFML/Graphics.hpp>
#include "PTree.hpp"

PTree::PTree(float L, int N) {
    baseSize = L;
    maxDepth = N;
    depth = N;

    float x = 3.0 * L;         // center of 6L width
    float y = 3.5 * L;         // bottom edge of 4L

    buildTree(x, y, L, 0.0f, N);
}

void PTree::buildTree(float x, float y, float size, float rotation, int depth) {
    if (depth == 0) return;

    // create and configure current square
    sf::RectangleShape square(sf::Vector2f(size, size));
    square.setOrigin({size/2.0f, size/2.0f});
    square.setPosition({x, y});
    square.setRotation(sf::degrees(rotation));

    // sakura tree colors: brown for base, pink grad starting at iteration 2
    sf::Color color;

    if (depth == maxDepth) {
        // base square only: dark brown trunk
        color = sf::Color(101, 67, 33);
    } else {
        // all other squares: gradient from reddish-brown to light pink
        // normalize depth for gradient (maxDepth-1 to 1)
        float depthRatio = 1.0f - (static_cast<float>(depth - 1) /
         static_cast<float>(maxDepth - 1));

        if (depthRatio < 0.33f) {
            // lower branches: reddish brown
            float t = depthRatio / 0.33f;
            color = sf::Color(
                139 + t * (219 - 139),
                69 + t * (112 - 69),
                19 + t * (147 - 19) );
        } else if (depthRatio < 0.67f) {
            // mid branches: deep pink
            float t = (depthRatio - 0.33f) / 0.34f;
            color = sf::Color(
                219 + t * (255 - 219),
                112 + t * (150 - 112),
                147 + t * (180 - 147) );
        } else {
            // outer edges: light sakura pink
            float t = (depthRatio - 0.67f) / 0.33f;
            color = sf::Color(
                255,
                150 + t * (183 - 150),
                180 + t * (197 - 180) );
        }
    }

    square.setFillColor(color);
    square.setOutlineColor(sf::Color(80, 50, 30));
    square.setOutlineThickness(1.0f);

    squares.push_back(square);

    // calculate child size and get parent's top-center
    float childSize = size / std::sqrt(2.0f);
    float rotRad = rotation * M_PI / 180.0f;
    float topCenterX = x - std::sin(rotRad) * (size / 2.0f);
    float topCenterY = y - std::cos(rotRad) * (size / 2.0f);

    // left child: rotation - 45°
    float leftRot = rotation - 45.0f;
    float leftRotRad = leftRot * M_PI / 180.0f;
    buildTree(
        topCenterX - std::sin(leftRotRad) * childSize,
        topCenterY - std::cos(leftRotRad) * childSize,
        childSize,
        leftRot,
        depth - 1);

    // right child: rotation + 45°
    float rightRot = rotation + 45.0f;
    float rightRotRad = rightRot * M_PI / 180.0f;
    buildTree(
        topCenterX - std::sin(rightRotRad) * childSize,
        topCenterY - std::cos(rightRotRad) * childSize,
        childSize,
        rightRot,
        depth - 1);
}

void PTree::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (const auto& square : squares) {
        target.draw(square, states);
    }
}
