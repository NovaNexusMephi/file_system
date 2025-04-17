#include <exception>
#include <iostream>
#include <nlohmann/json.hpp>
#include "monitor/reader.h"
#include "monitor/writer.h"

int main() {
    try {
        monitor::Reader reader{"> "};
        monitor::Writer writer{};

        while (true) {
            auto precommand = reader.read(std::cin);
            writer.write(std::cout, (precommand.has_value() ? "parsed" : "nullopt"));
        }

    } catch (const std::exception& fatal_error) {
        std::cout << fatal_error.what() << std::endl;
        return 1;
    }
    return 0;
}