// Copyright 2026 Dr. Daly
// Editted by Salma Boukdad
// Unit tests for PS1a
// Date: 2/1/26

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE SpriteSheet Tests

#include <iostream>
#include <string>

#include <boost/test/unit_test.hpp>

#include "SpriteSheet.hpp"

using sb::SpriteSheet;
using sb::TextureView;
using sf::Image;       // SFML Image object
using sf::Vector2u;    // 2D vector of unsigned ints

// ostream operator for Vector2u so we can compare equality easily
namespace sf {
std::ostream& operator<<(std::ostream& os, const sf::Vector2u& v) {
    os << "<" << v.x << "," << v.y << ">";
    return os;
}

bool operator==(const Image& a, const Image& b) {
    // check if image dimensions are the same
    if (a.getSize() != b.getSize()) {
        return false;
    }

    // check if pixel data is the same
    const auto pixelsA = a.getPixelsPtr();
    const auto pixelsB = b.getPixelsPtr();

    // totalBytes can consider only x's dimensions since we cleared that a and
    // b share the same dimensions in the condition above
    /* multiplying by 4 b/c each pixel is 4 bytes */
    std::size_t totalBytes = a.getSize().x * a.getSize().y * 4;

    // loop through each byte of a and b
    for (size_t i = 0; i < totalBytes; i++) {
        if (pixelsA[i] != pixelsB[i]) return false;  // found byte that differs
    }
    return true;    // bytes match .: same image
}
}    // namespace sf


// test crop() - creates new TV showing portion relative to the view
BOOST_AUTO_TEST_CASE(TestCropSize) {
    Image img("images/playingCards.png");
    SpriteSheet sheet(img, {140, 190});
    TextureView tile = sheet[0];

    // crop a 50x50 region
    TextureView cropped = tile.crop(sf::IntRect({10, 10}, {50, 50}));
    Image croppedImg = cropped.toImage();

    BOOST_REQUIRE_EQUAL(croppedImg.getSize(), Vector2u(50, 50));
}


// test that crop is actually different from original
BOOST_AUTO_TEST_CASE(TestCropDifferent) {
    Image img("images/playingCards.png");
    SpriteSheet sheet(img, {140, 190});
    TextureView tile = sheet[0];

    Image fullTile = tile.toImage();
    Image croppedTile = tile.crop(sf::IntRect({10, 10}, {50, 50})).toImage();

    // croppedTile should be different size .: not equal
    BOOST_REQUIRE(!(fullTile == croppedTile));
}


// test linear index and grid index give same tile
BOOST_AUTO_TEST_CASE(TestIndexEquivalence) {
    Image img("images/playingCards.png");
    SpriteSheet sheet(img, {140, 190});

    Image linear0 = sheet[0].toImage();
    Image grid0 = sheet[{0, 0}].toImage();
    BOOST_REQUIRE(linear0 == grid0);

    // tile 1 should equal
    Image linear1 = sheet[1].toImage();
    Image grid1 = sheet[{1, 0}].toImage();
    BOOST_REQUIRE(linear1 == grid1);
}


// test that toImage() returns correct size
BOOST_AUTO_TEST_CASE(TestToImageSize) {
    Image img("images/playingCards.png");
    SpriteSheet sheet(img, {140, 190});

    Image tile = sheet[0].toImage();
    BOOST_REQUIRE_EQUAL(tile.getSize(), Vector2u(140, 190));
}

// test out of bounds index throws
BOOST_AUTO_TEST_CASE(TestInvalidIndex) {
    Image img("images/playingCards.png");
    SpriteSheet sheet(img, {140, 190});

    BOOST_CHECK_THROW(sheet[sheet.length()], std::out_of_range);
    BOOST_CHECK_THROW(sheet[999], std::out_of_range);
}

// test invalid grid index throws
BOOST_AUTO_TEST_CASE(TestInvalidGridIndex) {
    Image img("images/playingCards.png");
    SpriteSheet sheet(img, {140, 190});

    BOOST_CHECK_THROW(sheet[Vector2u(sheet.width(), 0)], std::out_of_range);
    BOOST_CHECK_THROW(sheet[Vector2u(0, sheet.height())], std::out_of_range);
    BOOST_CHECK_THROW(sheet[Vector2u(999, 999)], std::out_of_range);
}

// test that different tiles are actually different
BOOST_AUTO_TEST_CASE(TestDifferentTiles) {
    Image img("images/playingCards.png");
    SpriteSheet sheet(img, {140, 190});

    Image tile0 = sheet[0].toImage();
    Image tile1 = sheet[1].toImage();

    BOOST_REQUIRE(!(tile0 == tile1));
}

// test invalid crop throws
BOOST_AUTO_TEST_CASE(TestInvalidCrop) {
    Image img("images/playingCards.png");
    SpriteSheet sheet(img, {140, 190});
    TextureView view = sheet[0];

    BOOST_CHECK_THROW(view.crop(sf::IntRect({150, 0}, {10, 10})),
        std::out_of_range);
    BOOST_CHECK_THROW(view.crop(sf::IntRect({0, 0}, {200, 200})),
        std::out_of_range);
}

// test row-major ordering
BOOST_AUTO_TEST_CASE(TestRowMajorOrdering) {
    Image img("images/playingCards.png");
    SpriteSheet sheet(img, {140, 190});

    // first tile of second row should be at index = width
    size_t width = sheet.width();
    Image tile13_linear = sheet[width].toImage();
    Image tile13_grid = sheet[{0, 1}].toImage();

    BOOST_REQUIRE(tile13_linear == tile13_grid);
}


BOOST_AUTO_TEST_CASE(TestMarginLogic) {
    Image img({100, 100}, sf::Color::Black);   // create 100 x 100 black img

    for (unsigned int x = 10; x < 15; x++) {
        img.setPixel({x, 0}, sf::Color::White);
    }

    SpriteSheet marginSheet(img, {10, 10}, 5);
    SpriteSheet noMargSheet(img, {10, 10}, 0);
    Image tile1_margin = marginSheet[1].toImage();
    Image tile1_noMarg = noMargSheet[1].toImage();

    // should be different bc margins shift where tile 1 starts
    BOOST_REQUIRE(!(tile1_margin == tile1_noMarg));
}

BOOST_AUTO_TEST_CASE(TestInstructorTileSize) {
    Image img("images/playingCards.png");
    SpriteSheet sheet(img, {140, 190});
    BOOST_REQUIRE_EQUAL(sheet.getTileSize(), Vector2u(140, 190));
}
