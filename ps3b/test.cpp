/****************************************************************************
 * Copyright 2026 Salma Boukdad
 * @Author: Salma Boukdad
 * @Course: COMP.2040.203
 * @Date: 2/26/26
 * @Description: This program instantiates a Sokoban obj by reading in a 
 *  level file, then issues commands and checks that the new state is
 *  what is expected. 
****************************************************************************/

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE

#include <iostream>
#include <string>

#include <boost/test/unit_test.hpp>

#include "Sokoban.hpp"
using SB::Direction;
using SB::Sokoban;

// @NOTE: do not create graphical window in tests

namespace sf {
// operator<< for Vector2u
std::ostream& operator<<(std::ostream& out, const sf::Vector2u& v) {
    return out << "(" << v.x << ", " << v.y << ")";
}
}

// @TEST: file parsing

// load a level, test dimensions (width and height)
    BOOST_AUTO_TEST_CASE(TestDimensions) {
        Sokoban game("MyLevels/level1.lvl");

        unsigned int width, height;
        width = game.width();
        height = game.height();

        BOOST_REQUIRE_EQUAL(width, 3);
        BOOST_REQUIRE_EQUAL(height, 5);
        BOOST_REQUIRE_EQUAL(width, game.size().x);
        BOOST_REQUIRE_EQUAL(height, game.size().y);
    }

// load a level, check dim == gridSize
    BOOST_AUTO_TEST_CASE(TestGridDimensions) {
        Sokoban game("MyLevels/level1.lvl");

        unsigned int width, height;
        width = game.width();
        height = game.height();

        BOOST_REQUIRE_EQUAL(width, game.size().x);
        BOOST_REQUIRE_EQUAL(height, game.size().y);
    }

// load a level, check playerLocation() is at expected pos
    BOOST_AUTO_TEST_CASE(TestPlayerLocation) {
        Sokoban game("levels/autograder/level1.lvl");
        sf::Vector2u playerPos = {3, 6};

        BOOST_REQUIRE_EQUAL(playerPos, game.playerLoc());
    }

//*****************************************************************************

// @TEST: basic player movement

// test UP: player y decreases by 1
    BOOST_AUTO_TEST_CASE(TestUp) {
        Sokoban game("levels/autograder/level1.lvl");

        game.movePlayer(Direction::Up);
        sf::Vector2u playerPos = {3, 5};

        BOOST_REQUIRE_EQUAL(playerPos, game.playerLoc());
    }

// test DOWN: player y increases by 1
    BOOST_AUTO_TEST_CASE(TestDown) {
        Sokoban game("MyLevels/level1.lvl");

        game.movePlayer(Direction::Down);
        sf::Vector2u playerPos = {1, 2};

        BOOST_REQUIRE_EQUAL(playerPos, game.playerLoc());
    }

// test LEFT: player x decreases by 1
    BOOST_AUTO_TEST_CASE(TestLeft) {
        Sokoban game("levels/autograder/level3.lvl");

        game.movePlayer(Direction::Left);
        sf::Vector2u playerPos = {2, 8};

        BOOST_REQUIRE_EQUAL(playerPos, game.playerLoc());
    }

// test RIGHT: player x increases by 1
    BOOST_AUTO_TEST_CASE(TestRight) {
        Sokoban game("levels/autograder/level3.lvl");

        game.movePlayer(Direction::Down);
        sf::Vector2u playerPos = {3, 9};

        BOOST_REQUIRE_EQUAL(playerPos, game.playerLoc());
    }

// test blocked direction: position unchanged
    BOOST_AUTO_TEST_CASE(TestBlockedDirection) {
        Sokoban game("MyLevels/level2.lvl");

        game.movePlayer(Direction::Up);
        sf::Vector2u playerPos = {1, 1};

        BOOST_REQUIRE_EQUAL(playerPos, game.playerLoc());
    }

//*****************************************************************************

// @TEST: box interactions

// test box pushed into open space, player and box move
    BOOST_AUTO_TEST_CASE(TestOpenSpace) {
        Sokoban game("levels/autograder/level5.lvl");

        BOOST_TEST_MESSAGE("Player Location Before Down: " <<
            game.playerLoc().x << ", " << game.playerLoc().y);
        game.movePlayer(Direction::Up);
        BOOST_TEST_MESSAGE("Player Location After Down: " <<
            game.playerLoc().x << ", " << game.playerLoc().y);
        sf::Vector2u playerPos = {1, 5};

        BOOST_REQUIRE_EQUAL(playerPos, game.playerLoc());
    }

// test box pushed onto storage area
    BOOST_AUTO_TEST_CASE(TestStoredBox) {
        Sokoban game("MyLevels/level4.lvl");

        game.movePlayer(Direction::Right);
        sf::Vector2u playerPos = {3, 2};

        BOOST_REQUIRE_EQUAL(playerPos, game.playerLoc());
    }

// test box push blocked by wall, nothing moves
    BOOST_AUTO_TEST_CASE(TestWallBox) {
        Sokoban game("levels/microban/level1.lvl");

        game.movePlayer(Direction::Left);
        sf::Vector2u playerPos = {2, 3};

        BOOST_REQUIRE_EQUAL(playerPos, game.playerLoc());
    }

// test box blocked by another box, nothing moves
    BOOST_AUTO_TEST_CASE(TestTwoBoxBlock) {
        Sokoban game("levels/microban/level9.lvl");

        game.movePlayer(Direction::Right);
        sf::Vector2u playerPos = {1, 2};

        BOOST_REQUIRE_EQUAL(playerPos, game.playerLoc());
    }

//*****************************************************************************

// @TEST: border interactions

// test player at top border, position unchange
    BOOST_AUTO_TEST_CASE(TestTopBorder) {
        Sokoban game("MyLevels/level7.lvl");

        game.movePlayer(Direction::Up);
        sf::Vector2u playerPos = {1, 1};

        BOOST_REQUIRE_EQUAL(playerPos, game.playerLoc());
    }

// test player at bottom border, position unchanged
    BOOST_AUTO_TEST_CASE(TestBottomBorder) {
        Sokoban game("MyLevels/level8.lvl");

        game.movePlayer(Direction::Down);
        sf::Vector2u playerPos = {4, 2};

        BOOST_REQUIRE_EQUAL(playerPos, game.playerLoc());
    }

// test player at left border, position unchanged
    BOOST_AUTO_TEST_CASE(TestLeftBorder) {
        Sokoban game("MyLevels/level5.lvl");

        game.movePlayer(Direction::Left);
        sf::Vector2u playerPos = {1, 2};

        BOOST_REQUIRE_EQUAL(playerPos, game.playerLoc());
    }

// test player at right border, position unchanged
    BOOST_AUTO_TEST_CASE(TestRightBorder) {
        Sokoban game("levels/microban/level6.lvl");

        game.movePlayer(Direction::Right);
        sf::Vector2u playerPos = {10, 2};

        BOOST_REQUIRE_EQUAL(playerPos, game.playerLoc());
    }

// test player on border tile and tries to go beyond board limits
    BOOST_AUTO_TEST_CASE(TestPlayerOnBorder) {
        Sokoban game("levels/autograder/walkover.lvl");

        game.movePlayer(Direction::Down);
        sf::Vector2u playerPos = {2, 5};

        BOOST_REQUIRE_EQUAL(playerPos, game.playerLoc());
    }

//*****************************************************************************

// @TEST: victory conditions

// test isWon() is false at start
    BOOST_AUTO_TEST_CASE(TestFalseAtStart) {
        Sokoban game("levels/autograder/pushright.lvl");

        BOOST_REQUIRE_EQUAL(false, game.isWon());
    }

// test isWon() is true when all boxes on storage
    BOOST_AUTO_TEST_CASE(TestRealWin) {
        Sokoban game("levels/autograder/pushright.lvl");
        game.movePlayer(Direction::Right);

        BOOST_REQUIRE_EQUAL(true, game.isWon());
    }

// test isWon() is false after reset()
    BOOST_AUTO_TEST_CASE(TestReset) {
        Sokoban game("levels/autograder/level1.lvl");

        game.movePlayer(Direction::Down);
        game.reset();

        sf::Vector2u playerPos = {3, 6};

        BOOST_REQUIRE_EQUAL(playerPos, game.playerLoc());
        BOOST_REQUIRE_EQUAL(false, game.isWon());
    }

// test isWon() is false when more boxes than targets
    BOOST_AUTO_TEST_CASE(TestManyBoxes) {
        std::stringstream ss("7 3\n#######\n#aAA@.#\n#######\n");
        Sokoban game;
        ss >> game;

        game.movePlayer(Direction::Left);

        BOOST_REQUIRE_EQUAL(false, game.isWon());
    }

// test isWon() is true when more storage than boxes
    BOOST_AUTO_TEST_CASE(TestManyTargets) {
        std::stringstream ss("7 3\n#######\n#1.a@.#\n#######\n");
        Sokoban game;
        ss >> game;

        BOOST_REQUIRE_EQUAL(true, game.isWon());
    }
