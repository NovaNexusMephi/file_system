#include "monitor/io/scanner.hpp"
#include <stdexcept>
#include <string>
using monitor::io::Scanner;

Scanner::Scanner(const std::string& s_name) {
    set_name(s_name);
}

std::string Scanner::get_name() const noexcept {
    return name_;
}

void Scanner::set_name(const std::string& new_name) {
    if (new_name.empty()) {
        throw std::invalid_argument("scanner name cannot be empty");
    }
    name_ = new_name;
}