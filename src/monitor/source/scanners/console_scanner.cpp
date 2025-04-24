#include "monitor/scanners/console_scanner.hpp"
#include <algorithm>
#include <iostream>
#include <istream>
#include <string>
using monitor::scanners::ConsoleScanner;

std::string ConsoleScanner::next() {
    std::string line;
    while (std::getline(std::cin, line, ';')) {
        line.erase(line.begin(),
                   std::find_if(line.begin(), line.end(), [](unsigned char c) { return !std::isspace(c); }));
        line.erase(std::find_if(line.rbegin(), line.rend(), [](unsigned char c) { return !std::isspace(c); }).base(),
                   line.end());
        if (!line.empty()) {
            return line;
        }
    }
    return "";
}

bool ConsoleScanner::has_next() const {
    return !std::cin.eof() && std::cin.good() && (std::cin.peek() != std::istream::traits_type::eof());
}