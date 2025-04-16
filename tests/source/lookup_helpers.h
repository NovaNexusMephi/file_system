#pragma once

#include <gtest/gtest.h>

#include <libconfig.h++>
namespace helpers {
template <typename T>
void add_setting(libconfig::Setting& parent, const std::string& name, T value);

template <>
inline void add_setting<int>(libconfig::Setting& parent, const std::string& name, int value) {
    parent.add(name, libconfig::Setting::TypeInt) = value;
}

template <>
inline void add_setting<std::string>(libconfig::Setting& parent, const std::string& name, std::string value) {
    parent.add(name, libconfig::Setting::TypeString) = value;
}

template <>
inline void add_setting<bool>(libconfig::Setting& parent, const std::string& name, bool value) {
    parent.add(name, libconfig::Setting::TypeBoolean) = value;
}
}  // namespace helpers

using LookupTestTypes = testing::Types<int, std::string, bool>;