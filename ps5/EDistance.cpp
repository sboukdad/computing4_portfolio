/***********************************************************
 * Copyright Salma Boukdad
 * 
 * Filename: EDistance.cpp
 * Course: COMP.2040.203
 * Date: 5/20/26
 * Description: This implements the EDistance
 *  class. This file utilizes dynamic programming
 *  to compute the edit distance between two DNA strings by
 *  filling an (M+1) * (N+1) cost matrix bottom-up. It
 *  provides penalty scoring, three-way minimum, optimal
 *  distane calculation, and alignment traceback with gap
 *  and mismatch costs.
************************************************************/

#include "EDistance.hpp"
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <string>

EDistance::EDistance(const std::string& s1, const std::string& s2) : x(s1),
 y(s2), M(s1.length()), N(s2.length()) {
    if (s1.empty() || s2.empty()) {
        throw std::invalid_argument("Strings cannot be empty");
    }

    // copy the vector
    opt.assign(M + 1, std::vector<int>(N + 1, 0));
}

int EDistance::penalty(char a, char b) {
    return (a == b) ? 0 : 1;
}

int EDistance::min3(int a, int b, int c) {
    return std::min({a, b, c});
}

// int EDistance::min3(int a, int b, int c) {
//     if (a <= b && a <= c) return a;
//     if (b <= c) return b;
//     return c;
// }

int EDistance::optDistance() {
    opt[M][N] = 0;

    // going from right most right most row up
    for (int i = M - 1; i >= 0; i--) {
        opt[i][N] = 2 * (M - i);
    }

    // going from bottom right most column to left
    for (int j = N - 1; j >= 0; j--) {
        opt[M][j] = 2 * (N - j);
    }

    for (int i = M - 1; i >= 0; i--) {
        for (int j = N - 1; j >= 0; j--) {
            opt[i][j] = min3(opt[i + 1][j + 1] + penalty(x[i], y[j]),
             opt[i + 1][j] + 2, opt[i][j + 1] + 2);
        }
    }

    return opt[0][0];
}

std::string EDistance::alignment() {
    std::string result = "";
    int i = 0, j = 0;

    while (i < M || j < N) {
        if (i < M && j < N &&
            opt[i][j] == opt[i + 1][j + 1] + penalty(x[i], y[j])) {
            // build string, i++, j++
            result += x[i];
            result += ' ';
            result += y[j];
            result += ' ';
            result += std::to_string(penalty(x[i], y[j]));
            result += '\n';
            i++;
            j++;
        } else if (i < M && opt[i][j] == opt[i + 1][j] + 2) {
            // build string, i++
            result += x[i];
            result += ' ';
            result += '-';
            result += ' ';
            result += '2';
            result += '\n';
            i++;
        } else {
        // build string, j++
        result += '-';
        result += ' ';
        result += y[j];
        result += ' ';
        result += '2';
        result += '\n';
        j++;
        }
    }
    return result;
}
