// Copyright Salma Boukdad
/****************************************************************
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
#include <vector>
#include <boost/test/unit_test.hpp>
#include "EDistance.hpp"

// test EDistance
BOOST_AUTO_TEST_CASE(TestConstructor) {
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
    BOOST_REQUIRE_EQUAL(EDistance::min3(5, 4, 3), 3);   // min last
    BOOST_REQUIRE_EQUAL(EDistance::min3(3, 3, 3), 3);   // all equal
    BOOST_REQUIRE_EQUAL(EDistance::min3(0, 1, 0), 0);   // tie
    BOOST_REQUIRE_EQUAL(EDistance::min3(3, 5, 4), 3);   // min first
    BOOST_REQUIRE_EQUAL(EDistance::min3(4, 3, 5), 3);   // min second

    BOOST_REQUIRE_EQUAL(EDistance::min3(100, 50, 75), 50);
    BOOST_REQUIRE_EQUAL(EDistance::min3(1000, 2000, 500), 500);
}

BOOST_AUTO_TEST_CASE(TestOptDistance) {
    EDistance obj("AACAGTTACC", "TAAGGTCA");
    BOOST_REQUIRE_EQUAL(obj.optDistance(), 7);

    EDistance obj2("ACGT", "ACGT");
    BOOST_REQUIRE_EQUAL(obj2.optDistance(), 0);

    EDistance obj3("A", "T");
    BOOST_REQUIRE_EQUAL(obj3.optDistance(), 1);

    EDistance obj4("AC", "CA");
    BOOST_REQUIRE_EQUAL(obj4.optDistance(), 2);
}

// test alignment
BOOST_AUTO_TEST_CASE(TestAlignment) {
    EDistance obj("AACAGTTACC", "TAAGGTCA");
    obj.optDistance();
    std::istringstream ss(obj.alignment());
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(ss, line)) lines.push_back(line);

    // check row count
    BOOST_REQUIRE_EQUAL(static_cast<int>(lines.size()), 10);

    // check first and last lines
    BOOST_REQUIRE_EQUAL(lines.front(), "A T 1");
    BOOST_REQUIRE_EQUAL(lines.back(), "C A 1");
    BOOST_REQUIRE_EQUAL(lines[2], "C - 2");

    // check total cost
    int cost = 0;
    for (const auto& l : lines) {
        std::istringstream ls(l);
        std::string a, b;
        int c;
        ls >> a >> b >> c;
        cost += c;
    }
    BOOST_REQUIRE_EQUAL(cost, 7);
}
