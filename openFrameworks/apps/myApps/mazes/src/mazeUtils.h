#pragma once

#include <string>

using namespace std;

inline string to_base36(unsigned int value) {
    static constexpr char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    if (value == 0)
        return "0";

    string result;
    while (value > 0) {
        result.push_back(digits[value % 36]);
        value /= 36;
    }
    reverse(result.begin(), result.end());

    return result;
}

inline string repeat(string_view str, size_t times) {
    string result;
    result.reserve(str.size() * times);
    for (size_t i = 0; i < times; ++i)
        result += str;

    return result;
}