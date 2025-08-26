#pragma once
#include <iostream>
#include <string>
#ifdef _WIN32
inline void clearScreen() { system("cls"); }
#else
inline void clearScreen() { std::cout << "\033[2J\033[1;1H"; }
#endif

inline bool readLine(std::string &out) {
    if (!std::getline(std::cin, out)) return false;
    if (!out.empty() && out.back() == '\r') out.pop_back();  // handle CRLF on Windows
    return true;
}