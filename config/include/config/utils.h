#ifndef UTILS_H
#define UTILS_H

#include <map>

// TODO: Replace this file into common library

template <typename Key, typename Value>
std::map<Value, Key> invert_map(const std::map<Key, Value>& input) {
  std::map<Value, Key> output;
  for (const auto& [key, value] : input) {
    output[value] = key;
  }
  return output;
}

#endif