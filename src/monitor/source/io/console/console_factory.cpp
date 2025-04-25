#include <memory>

#include "monitor/io/console/console_factory.hpp"
#include "monitor/io/console/console_printer.hpp"
#include "monitor/io/console/console_scanner.hpp"
#include "monitor/io/printer.hpp"
#include "monitor/io/scanner.hpp"
using monitor::io::Printer;
using monitor::io::Scanner;
using monitor::io::console::ConsoleFactory;

std::unique_ptr<Printer> ConsoleFactory::create_printer() const {
    return std::make_unique<ConsolePrinter>();
}

std::unique_ptr<Scanner> ConsoleFactory::create_scanner() const {
    return std::make_unique<ConsoleScanner>();
}