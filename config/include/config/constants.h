#ifndef CONFIG_CONSTANTS_H
#define CONFIG_CONSTANTS_H

namespace config {
constexpr int BLOCK_SIZE = 512;
constexpr int SEGMENT_SIZE_BLOCKS = 2;
constexpr int ENTRY_SIZE_WORDS = 8;
constexpr int SYSTEM_BLOCK = 1;
constexpr int CATALOG_START_BLOCK = 6;
constexpr int MAX_CATALOG_SEGMENTS = 31;
constexpr int INITIAL_SEGMENT = 1;
}  // namespace config

#endif