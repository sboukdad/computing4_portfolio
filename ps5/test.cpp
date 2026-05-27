/****************************************************************
 * Copyright Salma Boukdad
 *
 * Filename: test.cpp
 * Course: COMP.2040.203
 * Date: 5/20/26
 * Description: Unit tests for the EDistance
 *  class using Boost.Test. Tests cover constructor validation,
 *  penalty scoring, three-way minimum, optimal distance 
 *  computation, and alignment traceback cost accuracty against
 *  known DNA string pairs
 ***************************************************************/

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE DnaTests

#include <iostream>
#include <sstream>
#include <string>

#include <boost/test/unit_test.hpp>
#include "EDistance.hpp"

// test EDistance
BOOST_AUTO_TEST_CASE(TestConstructor) {
    BOOST_REQUIRE_THROW(EDistance("", "AGGTACAT"), std::invalid_argument);
    BOOST_REQUIRE_NO_THROW(EDistance("ACATTGACA", "ACATTGATA"));
}

// test penalty
BOOST_AUTO_TEST_CASE(TestPenalty) {
    int test1 = EDistance::penalty('A', 'T');       // should yield 1
    int test2 = EDistance::penalty('C', 'C');       // should yield 0

    BOOST_REQUIRE_EQUAL(test1, 1);
    BOOST_REQUIRE_EQUAL(test2, 0);
}

// test min3
BOOST_AUTO_TEST_CASE(TestMin3) {
    int test1 = EDistance::min3(3, 5, 4);       // test function against sets
    int test2 = EDistance::min3(2, 0, 1);
    int test3 = EDistance::min3(5, 8, 6);

    BOOST_REQUIRE_EQUAL(test1, 3);
    BOOST_REQUIRE_EQUAL(test2, 0);
    BOOST_REQUIRE_EQUAL(test3, 5);
}

// test optDistance
BOOST_AUTO_TEST_CASE(TestOptDistance) {
    std::string a = "AACAGTTACC";
    std::string b = "TAAGGTCA";

    EDistance obj(a, b);
    int optDistance = obj.optDistance();

    BOOST_REQUIRE_EQUAL(optDistance, 7);
}

// test alignment
BOOST_AUTO_TEST_CASE(TestAlignment) {
    std::string x = "AACAGTTACC";
    std::string y = "TAAGGTCA";

    EDistance obj(x, y);
    obj.optDistance();
    std::string newString = obj.alignment();
    std::istringstream ss(newString);

    std::string line;
    int cost = 0;
    while (std::getline(ss, line)) {
        std::string a, b;
        int rowCost;

        std::istringstream lineStream(line);
        lineStream >> x >> y >> rowCost;
        cost += rowCost;
    }

    BOOST_REQUIRE_EQUAL(cost, 7);
}
