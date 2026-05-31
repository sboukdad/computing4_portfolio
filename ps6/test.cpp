/**********************************************
 * Copyright Salma Boukdad
 * 
 * @Filename: test.cpp
 * @Course: COMP.2040.203
 * @Date: 5/29/26
 * @Descripion:
 * 
***********************************************/

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE RandWriter
#include <iostream>
#include <string>
#include <cmath>
#include <boost/test/unit_test.hpp>
#include "RandWriter.hpp"

// ----------------------Constructor-----------------------
// test constructor
BOOST_AUTO_TEST_CASE(TestConstructor) {
    RandWriter rw("gagggagaggagagaaa", 2);

    BOOST_REQUIRE_EQUAL(rw.orderK(), 2);
}

BOOST_AUTO_TEST_CASE(TestConstructorBadInput) {
    // text is shorter than k and should throw logic error
    BOOST_REQUIRE_THROW(RandWriter("ab", 5), std::logic_error);
}

// ----------------------freq(kgram)-----------------------------
// test freq
BOOST_AUTO_TEST_CASE(TestFreqKgram) {
    RandWriter rw("abbaaabbabababaabbaba", 2);

    // "ab" appears multiple times
    BOOST_REQUIRE(rw.freq("ab") > 0);
    BOOST_REQUIRE_EQUAL(rw.freq("ab"), 7);
}

BOOST_AUTO_TEST_CASE(TestFreqKgramNotFound) {
    RandWriter rw("abbaaabbabababaabbaba", 2);

    // "zz" never appears so should return 0
    BOOST_REQUIRE_EQUAL(rw.freq("zz"), 0);
}

BOOST_AUTO_TEST_CASE(TestFreqKgramWrongLength) {
    RandWriter rw("abbaaabbabababaabbaba", 2);

    // kgram of wrong length should throw
    BOOST_REQUIRE_THROW(rw.freq("a"), std::invalid_argument);
    BOOST_REQUIRE_THROW(rw.freq("aba"), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(TestFreqKgramOrder0) {
    RandWriter rw("abbaaabbabababaabbaba", 0);

    // order 0: freq("") should return length of text
    BOOST_REQUIRE_EQUAL(rw.freq(""), 21);
}

BOOST_AUTO_TEST_CASE(TestFreqKgramChar) {
    RandWriter rw("abbaaabbabababaabbaba", 2);

    // verify specific character frequencies
    BOOST_REQUIRE(rw.freq("ab", 'a') > 0);
    BOOST_REQUIRE(rw.freq("ab", 'b') > 0);
}

BOOST_AUTO_TEST_CASE(TestFreqKgramCharNotFound) {
    RandWriter rw("abbaaabbabababaabbaba", 2);

    // 'z' never follows "ga"
    BOOST_REQUIRE_EQUAL(rw.freq("ab", 'z'), 0);
}

BOOST_AUTO_TEST_CASE(TestFreqKgramCharWrongLength) {
    RandWriter rw("abbaaabbabababaabbaba", 2);

    BOOST_REQUIRE_THROW(rw.freq("g", 'a'), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(TestFreqKgramCharOrder0) {
    RandWriter rw("abbaaabbabababacbbaba", 0);

    // order 0: count occurences of 'a' in text
    BOOST_REQUIRE_EQUAL(rw.freq("", 'a'), 10);
    BOOST_REQUIRE_EQUAL(rw.freq("", 'c'), 1);
    BOOST_REQUIRE_EQUAL(rw.freq("", 'b'), 10);
}

// ------------------------kRand-------------------------
// test kRand()
BOOST_AUTO_TEST_CASE(TestKrandValidChar) {
    RandWriter rw("abbaaabbabababaabbaba", 2);

    // kRand should return a char that actually follows "ab"
    char c = rw.kRand("ab");
    BOOST_REQUIRE(c == 'b' || c == 'a');
}

BOOST_AUTO_TEST_CASE(TestKrandWrongLength) {
    RandWriter rw("abbaaabbabababaabbaba", 2);

    // wrong kgram length should throw
    BOOST_REQUIRE_THROW(rw.kRand("g"), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(TestKRandNotFound) {
    RandWriter rw("abbaaabbabababaabbaba", 2);

    // kgram not in text should throw
    BOOST_REQUIRE_THROW(rw.kRand("zz"), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(TestKRandNoThrow) {
    RandWriter rw("abbaaabbabababaabbaba", 2);

    // valid kgram should not throw
    BOOST_REQUIRE_NO_THROW(rw.kRand("ab"));
}

// ---------------------------Generate----------------------
// test generate results
BOOST_AUTO_TEST_CASE(TestGenerateLength) {
    RandWriter rw("abbaaabbabababaabbaba", 2);
    std::string result = rw.generate("ab", 100);

    // output must be exactly length L
    BOOST_REQUIRE_EQUAL(result.size(), 100);
}

BOOST_AUTO_TEST_CASE(TestGenerateStartWithKgram) {
    RandWriter rw("abbaaabbabababaabbaba", 2);
    std::string result = rw.generate("ab", 50);

    // first k chars must be the seed kgram
    BOOST_REQUIRE_EQUAL(result.substr(0, 2), std::string("ab"));
}

BOOST_AUTO_TEST_CASE(TestGenerateWrongLength) {
    RandWriter rw("abbaaabbabababaabbaba", 2);

    // wrong kgram length should throw
    BOOST_REQUIRE_THROW(rw.generate("a", 50), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(TestGenerateNoThrow) {
    RandWriter rw("abbaaabbabababaabbaba", 2);
    BOOST_REQUIRE_NO_THROW(rw.generate("ab", 10));
}

BOOST_AUTO_TEST_CASE(TestGenerateValidChars) {
    RandWriter rw("abbaaabbabababcabbaba", 2);
    std::string result = rw.generate("ab", 50);

    // every char in result should be in alphabet
    for (char c : result) {
        BOOST_REQUIRE(c == 'a' || c == 'b' || c == 'c');
    }
}

// -----------------------Letter Frequency----------------
// test letter frequency
BOOST_AUTO_TEST_CASE(TestKRandFrequency) {
    RandWriter rw("abbaaabbabababcabbaba", 2);

    // get expected frequencies
    int freqA = rw.freq("ab", 'a');
    int freqB = rw.freq("ab", 'b');
    int total = rw.freq("ab");

    // run kRand many times and count results
    int countA = 0;
    int countB = 0;
    int trials = 10000;

    for (int i = 0; i < trials; i++) {
        char c = rw.kRand("ab");
        if (c == 'a') countA++;
        if (c == 'b') countB++;
    }

    // check that observed ratio is within 5% of expected ratio
    double expectedA = static_cast<double>(freqA) / total;
    double observedA = static_cast<double>(countA) / trials;

    double expectedB = static_cast<double>(freqB) / total;
    double observedB = static_cast<double>(countB) / trials;

    BOOST_REQUIRE(std::abs(observedA - expectedA) < 0.05);
    BOOST_REQUIRE(std::abs(observedB - expectedB) < 0.05);
}
