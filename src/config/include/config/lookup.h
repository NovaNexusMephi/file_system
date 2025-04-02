#ifndef CONFIG_LOOKUP_H
#define CONFIG_LOOKUP_H

#include <functional>
#include <libconfig.h++>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace config {

/**
 * @brief Attempts to retrieve a scalar value of type T from a libconfig::Setting.
 *
 * This function checks if the key `as` exists in `from` and can be converted to type `T`.
 * Returns `std::nullopt` if the key is missing or the value is incompatible.
 *
 * @tparam T The type of the scalar value (e.g., `int`, `std::string`).
 * @param[in] from The `libconfig::Setting` object to query.
 * @param[in] as The key to retrieve from the setting.
 * @return std::optional<T> The retrieved value if successful, or `std::nullopt` otherwise.
 */
template <typename T>
[[nodiscard]] std::optional<T> scalar(const libconfig::Setting& from, const std::string& as) noexcept {
    if (!from.exists(as))
        return std::nullopt;

    T scalar_value;
    if (from.lookupValue(as, scalar_value))
        return scalar_value;
    return std::nullopt;
}

/**
 * @brief Retrieves a scalar value of type T, enforcing its presence and validity.
 *
 * Throws an exception if the key is missing or the value cannot be converted to `T`.
 *
 * @tparam T The type of the scalar value (e.g., `int`, `std::string`).
 * @param[in] from The `libconfig::Setting` object to query.
 * @param[in] as The key to retrieve from the setting.
 * @return T The retrieved value.
 * @throw std::runtime_error If the key is missing or has an incompatible type.
 */
template <typename T>
[[nodiscard]] T required_scalar(const libconfig::Setting& from, const std::string& as, bool required) {
    auto scalar_value = required_scalar<T>(from, as);
    if (scalar_value == std::nullopt && required) {
        throw std::runtime_error("invalid configuration: " + as + " is missing.");
    } else if (required && from.exists(as)) {
        throw std::runtime_error("invalid configuration: " + as + " has an incompatible type.");
    } else if (scalar_value == std::nullopt) {
        return T{};
    }
    return *scalar_value;
}

/**
 * @brief Retrieves a scalar value of type T and validates it against a predicate.
 *
 * Throws an exception if the key is missing, the value is incompatible, or validation fails.
 *
 * @tparam T The type of the scalar value (e.g., `int`, `std::string`).
 * @param[in] from The `libconfig::Setting` object to query.
 * @param[in] as The key to retrieve from the setting.
 * @param[in] predicate A validation function (returns `true` if the value is acceptable).
 * @return T The validated value.
 * @throw std::runtime_error If the key is missing, has an incompatible type, or fails validation.
 */
template <typename T>
[[nodiscard]] T validated_scalar(const libconfig::Setting& from, const std::string& as,
                                 std::function<bool(const T&)> predicate) {
    T value = required_scalar<T>(from, as, true);
    if (!predicate(value)) {
        throw std::runtime_error("invalid configuration: " + as + " failed validation.");
    }
    return value;
}

/**
 * @brief Retrieves a scalar value of type T, returning a default value if missing or invalid.
 *
 * If the key is missing, the value is incompatible, or an error occurs, `default_value` is returned.
 *
 * @tparam T The type of the scalar value (e.g., `int`, `std::string`).
 * @param[in] from The `libconfig::Setting` object to query.
 * @param[in] as The key to retrieve from the setting.
 * @param[in] default_value The fallback value if the key is invalid or missing.
 * @return T The retrieved value or `default_value`.
 */
template <typename T>
[[nodiscard]] T scalar_of_default(const libconfig::Setting& from, const std::string& as, T default_value) {
    auto scalar_value = scalar<T>(from, as);  // Use optional-based lookup
    return scalar_value.value_or(default_value);
}

/**
 * @brief Attempts to retrieve an array of values from a libconfig::Setting object.
 *
 * This function checks if the specified key (`as`) exists in the `from` setting and if the
 * associated value is of type `libconfig::Setting::TypeArray`. If these conditions are met,
 * the function iterates through the array, extracting each element as type `T`. The extracted
 * elements are stored in a `std::vector<T>` and returned wrapped in a `std::optional<std::vector<T>>`.
 *
 * If the key does not exist, the value is not an array, or any element cannot be extracted as type `T`,
 * the function returns `std::nullopt`. Additionally, if a `libconfig::SettingException` is thrown during
 * the lookup process, the function will also return `std::nullopt`.
 *
 * @tparam T The type of the elements in the array (e.g., int, double, std::string).
 * @param[in] from The libconfig::Setting object to query.
 * @param[in] as The name of the key to look up in the setting.
 * @return std::optional<std::vector<T>> The retrieved array of values if successful, or `std::nullopt` otherwise.
 */
template <typename T>
[[nodiscard]] std::optional<std::vector<T>> vector(const libconfig::Setting& from, const std::string& as) {
    if (!from.exists(as)) {
        return std::nullopt;
    }

    try {
        const auto& values = from.lookup(as);
        if (values.getType() != libconfig::Setting::TypeArray) {
            return std::nullopt;
        }

        std::vector<T> linear_value;
        linear_value.reserve(values.getLength());

        for (int i = 0; i < values.getLength(); ++i) {
            T elem;
            if (!values[i].lookupValue(elem)) {
                return std::nullopt;
            }
            linear_value.push_back(std::move(elem));
        }

        return linear_value;
    } catch (const libconfig::SettingException&) {
        return std::nullopt;
    }
}

/**
 * @brief Retrieves an array of values from a libconfig::Setting, with an option to enforce its presence and validity.
 *
 * This function queries the `libconfig::Setting` for an array associated with the key `as`. 
 * The behavior depends on the `required` parameter:
 * - If `required` is `true`:
 *   - Throws `std::runtime_error` if the key is missing.
 *   - Throws `std::runtime_error` if the key exists but:
 *     - The value is not an array, or
 *     - Any element in the array cannot be converted to type `T`.
 * - If `required` is `false`:
 *   - Returns an empty `std::vector<T>` if the key is missing, the value is not an array, or elements are incompatible.
 *
 * @tparam T The type of the elements in the array (e.g., `int`, `double`, `std::string`).
 * @param[in] from The `libconfig::Setting` object to query.
 * @param[in] as The key to look up in the setting.
 * @param[in] required Specifies whether the key must exist and be a valid array. 
 *                    If `true`, missing keys or invalid arrays trigger exceptions.
 * @return std::vector<T> The retrieved array if successful. If `required` is `false` and the value is missing/invalid, returns an empty vector.
 * @throw std::runtime_error If `required` is `true` and:
 *         - The key is missing (`"invalid configuration: [key] is missing."`), or
 *         - The key exists but is not an array or contains incompatible elements (`"invalid configuration: [key] has an incompatible type."`).
 */
template <typename T>
[[nodiscard]] std::vector<T> required_vector(const libconfig::Setting& from, const std::string& as, bool required) {
    auto vector_value = vector<T>(from, as);
    if (vector_value == std::nullopt && required) {
        throw std::runtime_error("invalid configuration: " + as + " is missing.");
    } else if (required && from.exists(as)) {
        throw std::runtime_error("invalid configuration: " + as + " has an incompatible type.");
    } else if (vector_value == std::nullopt) {
        return std::vector<T>{};
    }
    return *vector_value;
}

/**
 * @brief Retrieves an array of values of type T and validates each element against a predicate.
 *
 * This function retrieves the array associated with the key `as`, ensures it is valid (exists, is an array, and all elements are type `T`),
 * then validates each element using the provided predicate. If any element fails validation, an exception is thrown.
 *
 * @tparam T The type of the elements in the array (e.g., `int`, `std::string`).
 * @param[in] from The `libconfig::Setting` object to query.
 * @param[in] as The key to retrieve from the setting.
 * @param[in] predicate A validation function (returns `true` if the element is acceptable).
 * @return std::vector<T> The validated array.
 * @throw std::runtime_error If:
 *         - The key is missing, not an array, or contains incompatible elements, or
 *         - Any element fails the predicate check.
 */
template <typename T>
[[nodiscard]] std::vector<T> validated_vector(const libconfig::Setting& from, const std::string& as,
                                              std::function<bool(const T&)> predicate) {
    std::vector<T> vec = required_vector<T>(from, as, true);
    for (const auto& elem : vec) {
        if (!predicate(elem)) {
            throw std::runtime_error("invalid configuration: " + as + " failed validation.");
        }
    }
    return vec;
}

/**
 * @brief Retrieves an array of values of type T, returning a default value if missing or invalid.
 *
 * If the key is missing, the value is not an array, or any element is incompatible with `T`, the provided `default_value` is returned.
 *
 * @tparam T The type of the elements in the array (e.g., `int`, `std::string`).
 * @param[in] from The `libconfig::Setting` object to query.
 * @param[in] as The key to retrieve from the setting.
 * @param[in] default_value The fallback value if the key is invalid or missing.
 * @return std::vector<T> The retrieved array if valid, or `default_value` otherwise.
 */
template <typename T>
[[nodiscard]] std::vector<T> vector_or_default(const libconfig::Setting& from, const std::string& as,
                                               const std::vector<T>& default_value) {
    auto vec = vector<T>(from, as);
    return vec.value_or(default_value);
}

}  // namespace config

#endif