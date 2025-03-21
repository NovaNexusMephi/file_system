#include "config/catalog_config.h"

#include <libconfig.h++>
#include <stdexcept>

#include "config/constants.h"
#include "config/field_lookup.h"
using config::CatalogConfig;

#define MAX_SEGMENTS_FIELD "max_segments"
#define SEGMENT_SIZE_BLOCKS_FIELD "segment_size_blocks"
#define ENTRY_SIZE_WORDS_FIELD "entry_size_words"

CatalogConfig::CatalogConfig(const libconfig::Setting& setting) {
    lookup_scalar_field(setting, MAX_SEGMENTS_FIELD, max_segments);
    if (max_segments > MAX_CATALOG_SEGMENTS) {
        throw std::runtime_error("invalid catalog configuration: max_segments exceeds limit");
    }

    lookup_scalar_field(setting, SEGMENT_SIZE_BLOCKS_FIELD, segment_size_blocks);
    if (segment_size_blocks != SEGMENT_SIZE_BLOCKS) {
        throw std::runtime_error(
            "invalid catalog configuration: segment_size_blocks must be equal to "
            "SEGMENT_SIZE_BLOCKS");
    }

    lookup_scalar_field(setting, ENTRY_SIZE_WORDS_FIELD, entry_size_words);
    if (entry_size_words != ENTRY_SIZE_WORDS) {
        throw std::runtime_error(
            "invalid catalog configuration: entry_size_word must be equal to "
            "ENTRY_SIZE_WORDS");
    }
}