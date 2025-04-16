#pragma once

#include <libconfig.h++>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace config {

template <typename T>
[[nodiscard]] std::optional<T> get(const libconfig::Setting& from, const std::string& as) noexcept {
    using ValueType = std::conditional_t<std::is_const_v<T>, std::remove_const_t<T>, T>;

    if (!from.exists(as))
        return std::nullopt;

    ValueType lookup_value;
    try {
        if (from.lookupValue(as, lookup_value))
            return lookup_value;
    } catch (...) {
        return std::nullopt;
    }

    return std::nullopt;
};

template <typename T>
[[nodiscard]] T get_default(const libconfig::Setting& from, const std::string& as, T& default_value) noexcept {
    if (auto opt = get<T>(from, as); opt.has_value()) {
        return opt.value();
    }
    return default_value;
};

template <typename T>
[[nodiscard]] T get_required(const libconfig::Setting& from, const std::string& as) {
    auto lookup_res = get<T>(from, as);
    if (!lookup_res.has_value() && from.exists(as)) {
        throw std::runtime_error{"invalid configuration: " + as + " has an incompatible type."};
    } else if (!lookup_res.has_value()) {
        throw std::runtime_error{"invalid configuration: " + as + " is missing."};
    }
    return lookup_res.value();
}

}  // namespace config