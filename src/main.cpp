#include <exception>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include "monitor/core/reader.hpp"
#include "monitor/core/reader_manager.hpp"
#include "monitor/readers/console_reader.hpp"

using monitor::core::Reader;
using monitor::core::ReaderManager;
using monitor::readers::ConsoleReader;

int main() {
    try {
        std::unique_ptr<Reader> reader = std::make_unique<ConsoleReader>();
        ReaderManager manager{std::move(reader)};

        while (true) {
            auto msg = manager.get();
            std::cout << msg;
        }

    } catch (const std::exception& fatal_error) {
        std::cout << fatal_error.what() << std::endl;
        return 1;
    }
    return 0;
}