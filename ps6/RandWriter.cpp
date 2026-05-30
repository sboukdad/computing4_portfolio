/**********************************************************
 * Copyright Salma Boukdad
 * 
 * @Filename: RandWriter.cpp
 * @Course: COMP4.2040.203
 * @Date: 5/28/26
 * @Description:
 * 
***********************************************************/

#include "RandWriter.hpp"

RandWriter::RandWriter(const std::string& str, size_t k) {
    // initialize Markov model map
    this->k = k;
    text = str;

    // make text circular by appending first k chars to end
    std::string circular = text + text.substr(0, k);

    // sliding window
    for (size_t i = 0; i < text.size(); i++) {
        // create substring with window
        std::string kgram = circular.substr(i, k);
        char next = circular[i + k];            // get next char in string

        // update map
        freqMap[kgram][next]++;

        // print kgram
        std::cout << "kgram: \"" << kgram << "\" -> next: '" << next << "'\n";
        
        if (alphabet.find(next) == std::string::npos) {
            alphabet += next;
        }
    }

    rng = std::mt19937(std::chrono::system_clock::now().time_since_epoch().count());
}

// order k of Markov model
size_t RandWriter::orderK() const {
    return k;
}

// number of occurences of kgram (specific string window) in txt
int RandWriter::freq(const std::string& kgram) const {
    // error check
    if (kgram.size() != k) {
        throw std::invalid_argument("kgram must be of length k");
    }

    // order 0 edge case
    if (k == 0) {
        return text.size();
    }

    // find kgram in outer map
    auto it = freqMap.find(kgram);
    if (it == freqMap.end()) {
        return 0;
    }

    // sum all character frequencies for this kgram
    int total = 0;
    for (auto& pair : it->second) {
        total += pair.second;
    }

    return total;
}

// number of times char c follows kgram
int RandWriter::freq(const std::string& kgram, char c) const {
    // error check
    if (kgram.size() != k) {
        throw std::invalid_argument("kgram must be of length k");
    }

    // order 0 edge case: count occurrences of c in text
    if (k == 0) {
        return text.size();
    }

    // find kgram in outer map
    auto it = freqMap.find(kgram);
    if (it == freqMap.end()) {
        return 0;
    }

    // find char in inner map
    auto charIt = it->second.find(c);
    if (charIt == it->second.end()) {
        return 0;
    }

    return charIt->second;
}

// generate random char to follow given kgram
char RandWriter::kRand(const std::string& kgram) {
    // error check
    if (kgram.size() != k) {
        throw std::invalid_argument("kgram must be of length k");
    }

    // error check
    auto it = freqMap.find(kgram);
    if (it == freqMap.end()) {
        throw std::invalid_argument("kgram not found in text");
    }

    // count total times anything followed this kgram
    int total = freq(kgram);    

    // roll a random number between 1 and total
    std::uniform_int_distribution<int> dist(1, total);
    int roll = dist(rng);

    // walk through alphabet and accumulate frequencies
    // return the char whose cummulative range the roll falls into
    int cummulative = 0;
    for (char c : alphabet) {
        cummulative += freq(kgram, c);
        if (roll <= cummulative) {
            return c;
        }
    }

    throw std::runtime_error("krand failed to pick a character");
}

std::string RandWriter::generate(const std::string& kgram, size_t l) {
    // error check
    if (kgram.size() != k) {
        throw std::invalid_argument("kgram must be of length k");
    }

    // get string we are starting with
    std::string result = kgram;
    while (result.size() < l) {
        std::string current = result.substr(result.size() - k, k);
        result += kRand(current);
    }

    return result;
}

std::ostream& operator<<(std::ostream& out, const RandWriter& rw) {
    // print: order k
    // alphabet
    // kgram : frequencies ["kgram"=int] ...
    out << "Order: " << rw.k << std::endl;              // print order k
    out << "Alphabet: " << rw.alphabet << std::endl;    // print alphabet
    
    for (auto& pair : rw.freqMap) {           // print all frequencies in kgram
        out << "\"" << pair.first << "\": ";

        for (auto& kPair : pair.second) {
            // k+1 gram is pair + the next char
            std::string newPair = pair.first + kPair.first;
            out << "\"" << newPair << "\"=" << kPair.second << " ";
        }
        out << "\n";
    }

    return out;
}
