#ifndef CONFIG_FIELD_LOOKUP_H
#define CONFIG_FIELD_LOOKUP_H

#include <libconfig.h++>
#include <stdexcept>
#include <vector>

namespace config {

template <typename T>
[[maybe_unused]] inline bool lookup_scalar_field(const libconfig::Setting& setting, const std::string& field_name,
                                                 T& field, bool required = false) {
    bool contains_field = setting.exists(field_name);
    if (required && !contains_field) {
        throw std::runtime_error("invalid configuration: " + field_name + "is required but it is missing");
    }
    if (contains_field) {
        setting.lookupValue(field_name, field);
    }
    return contains_field;
}

template <typename T>
[[maybe_unused]] inline bool lookup_vector_field(const libconfig::Setting& setting, const std::string& field_name,
                                                 std::vector<T>& field, bool required = false) {
    bool contains_field = setting.exists(field_name);
    if (required && !contains_field) {
        throw std::runtime_error("invalid configuration: " + field_name + "is required but it is missing");
    }
    if (contains_field) {
        const libconfig::Setting& values = setting[field_name];
        for (int i = 0; i < values.getLength(); i++) {
            field.push_back(values[i]);
        }
    }
    return contains_field;
}

}  // namespace config

#endif