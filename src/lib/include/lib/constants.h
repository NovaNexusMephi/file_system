#ifndef LIB_CONSTANTS_H
#define LIB_CONSTANTS_H

#include <string>
namespace lib {
/// Minimum number of segments in filesystem catalog.
constexpr unsigned short MIN_SEGMENT_NUMBER = 1;

/// Maximum number of segments in filesystem catalog.
constexpr unsigned short MAX_SEGMENT_NUMBER = 31;

constexpr unsigned short DEFAULT_SEGMENT_NUMBER = 15;

static const std::string DEFAULT_VOLUME_NAME = "VOL001";

static const std::string DEFAULT_OWNER_NAME = "Admin";

static const std::string DEFAULT_SYSTEM_NAME = "fs";

}  // namespace lib

#endif