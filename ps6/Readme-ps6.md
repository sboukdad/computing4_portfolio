# PS6: RandWriter

## Contact
Name: Salma Boukdad
Section: 203
Time to Complete: 1 day


## Description
RandWriter build an order-k Markov model from an input text and uses it to generate pseudo-random text. It analyzes the requency of k-length character sequences (k-grams) and the characters that follow them, then uses those probabilities to simulate a random walk through the modeland produce new text that resembles the original.

### Features
The core data structure is a nested std::map <std::string, std::map<char, int>>.
The outer map keys are k-grams and the inner map tracks how many times each character followed that k-gram in the original text. This made frequency lookups straightforward and the map auto-creates entries on the first access, simplifying the constructor.

The input text is treated as circular by appending the first k characters to the end before the sliding window traversal. This guarantees every k-gram has at least one valid successor, preventing dead ends during text generation.

For random character selection in kRand(), a Mersenne Twister (mt19937) is seeded once in the constructor using the system clock. A uniform distribution rolls number between 1 and the total frequency of the k-gram, then a cumulative walk through the alphabet selects the character whole frequency range the roll falls into. This mirrors the original character frequencies proportionally.

### Testing
Tests were written using Boost.Test covering all methods of the RandWriter class. Boost_REQUIRE_THROW was used to verify that invalid_argument is thrown when a k-gram of the wrong length is passes to freq(), kRand(), and generate(), and when kRand() is called with a kgram not found in the text. BOOST_REQUIRE_NO_THROW was used to verify valid inputs do not throw. A frequency test runs kRand() 10000 times verifies the observed character distribution stays within 5% of the expected distribution based on the freq() map.

### Lambda
A lambda expression is used in the freq(kgram) method as a parameter to std::accumulate. The lambda takes a running sum and a std::pair<char, int> from the inner frequency map and adds the pair's integer value to the sum. This accumulates the total number of times any character followed the given k-gram in the original text. It replaces a manual for loop and is passed directly to std::accumulate from the <numeric> library.

### Issues
The most challenging part was handling the order-0 edge case, where there are no k-grams and frequency lookups must fall back to counting raw character occurrences in the original text. Getting the circular text wrapping correct in the constructor also required careful attention to the loop bounds and substr indices. Reading stdin with getline initially added newline characters into the model, which produced unexpected newlines in the generated output - fixed by removing the newline concatenation.

### Extra Credit

## Acknowledgements
- https://www.geeksforgeeks.org/cpp/searching-map-using-stdmap-functions-c/

- https://en.cppreference.com/cpp/numeric/random/uniform_int_distribution