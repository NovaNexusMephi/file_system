#pragma once

#include <libconfig.h++>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace config {

/**
 * @brief Attempts to retrieve a value of type T from a libconfig::Setting object.
 * 
 * This function checks if the specified key (`as`) exists in the `from` setting. 
 * If the key exists and its value can be successfully retrieved and converted to type T, 
 * the value is returned wrapped in a std::optional. If the key does not exist, 
 * or if an exception occurs during the lookup process, the function returns std::nullopt.
 * 
 * @tparam T The type of the value to retrieve.
 * @param[in] from The libconfig::Setting object to query.
 * @param[in] as The key (name) of the setting to retrieve.
 * @return std::optional<T> An optional containing the retrieved value if successful, 
 *         or std::nullopt if the value could not be retrieved.
 */
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

/**
 * @brief Retrieves a value of type T from a libconfig::Setting object, with a default fallback.
 * 
 * This function attempts to retrieve the value associated with the key (`as`) from the `from` setting. 
 * If the value is successfully retrieved, it is returned. If the value cannot be retrieved 
 * (e.g., the key does not exist or the type is incompatible), the provided `default_value` is returned instead.
 * 
 * @tparam T The type of the value to retrieve.
 * @param[in] from The libconfig::Setting object to query.
 * @param[in] as The key (name) of the setting to retrieve.
 * @param[in] default_value The fallback value to return if the lookup fails.
 * @return T The retrieved value if successful, or the `default_value` if the lookup fails.
 */
template <typename T>
[[nodiscard]] T get_default(const libconfig::Setting& from, const std::string& as, const T& default_value) noexcept {
    if (auto opt = get<T>(from, as); opt.has_value()) {
        return opt.value();
    }
    return default_value;
};

/**
 * @brief Retrieves a required value of type T from a libconfig::Setting object.
 * 
 * This function attempts to retrieve the value associated with the key (`as`) from the `from` setting. 
 * If the value is successfully retrieved, it is returned. If the key does not exist, 
 * or if the value has an incompatible type, an exception is thrown with a descriptive error message.
 * 
 * @tparam T The type of the value to retrieve.
 * @param[in] from The libconfig::Setting object to query.
 * @param[in] as The key (name) of the setting to retrieve.
 * @return T The retrieved value if successful.
 * @throws std::runtime_error If the key does not exist or the value has an incompatible type.
 */
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