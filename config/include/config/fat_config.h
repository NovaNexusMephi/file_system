#ifndef CONFIG_FAT_CONFIG_H
#define CONFIG_FAT_CONFIG_H

namespace config {

/// @addtogroup configuration
/// @{

// TODO: Replace this enum into common library

/// @enum SegmentNumberLimits
/// @brief Valid range for FAT segment numbers
/// @warning Values will be moved to common library [[1]]
enum SegmentNumberLimits : int {
    MIN_SEGMENT_NUMBER = 1,  ///< Minimum allowed segment count (inclusive)
    MAX_SEGMENT_NUMBER = 31  ///< Maximum allowed segment count (inclusive)
};

/// @struct FATConfig
/// @brief Represents configuration of file allocation table (FAT).
struct FATConfig final {
    int segment_number{};  ///< Number of segments (must be within [MIN_SEGMENT_NUMBER, MAX_SEGMENT_NUMBER])
};

/// @}
}  // namespace config

#endif