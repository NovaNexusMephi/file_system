#include "monitor/io/printer.hpp"
#include <stdexcept>
#include <string>
using monitor::io::Printer;

Printer::Printer(const std::string& p_name) {
    set_name(p_name);
}

std::string Printer::get_name() const noexcept {
    return name_;
}

void Printer::set_name(const std::string& new_name) {
    if (new_name.empty()) {
        throw std::invalid_argument("printer name cannot be empty");
    }
    name_ = new_name;
}