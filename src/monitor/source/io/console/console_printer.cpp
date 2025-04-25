#include <iostream>

#include "monitor/io/console/console_printer.hpp"
using monitor::io::console::ConsolePrinter;

void ConsolePrinter::print(const std::string& msg) {
    std::cout << msg;
}