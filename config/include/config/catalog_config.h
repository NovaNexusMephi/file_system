#ifndef CONFIG_CATALOG_CONFIG_H
#define CONFIG_CATALOG_CONFIG_H

#include <libconfig.h++>

#include "config/constants.h"

namespace config {
struct CatalogConfig final {
    int max_segments = MAX_CATALOG_SEGMENTS;
    int segment_size_blocks = SEGMENT_SIZE_BLOCKS;
    int entry_size_words = ENTRY_SIZE_WORDS;

    CatalogConfig() = default;
    explicit CatalogConfig(const libconfig::Setting& setting);
};

}  // namespace config

#endif