#include "monitor/logging/id_generator.hpp"
#include <random>

#include <chrono>

using monitor::logging::IDGenerator;

std::string IDGenerator::generate() noexcept {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> distrib(0, 9999);

    auto now = std::chrono::system_clock::now();

    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y%m%d%H%M%S") << "_" << std::setfill('0') << std::setw(4) << distrib(gen);

    return oss.str();
}
