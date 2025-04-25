#include <gtest/gtest.h>
#include <memory>
#include "monitor/io/console/console_factory.hpp"
#include "monitor/io/console/console_printer.hpp"
#include "monitor/io/console/console_scanner.hpp"

using monitor::io::console::ConsoleFactory;
using monitor::io::console::ConsolePrinter;
using monitor::io::console::ConsoleScanner;

TEST(ConsoleFactoryTest, CreatePrinterReturnsCorrectType) {
    ConsoleFactory factory;
    auto printer = factory.create_printer();
    ASSERT_TRUE(printer);
    EXPECT_NE(dynamic_cast<ConsolePrinter*>(printer.get()), nullptr);
}

TEST(ConsoleFactoryTest, CreateScannerReturnsCorrectType) {
    ConsoleFactory factory;
    auto scanner = factory.create_scanner();
    ASSERT_TRUE(scanner);
    EXPECT_NE(dynamic_cast<ConsoleScanner*>(scanner.get()), nullptr);
}