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
  lookup_scalar_field(setting, SEGMENT_SIZE_BLOCKS_FIELD, segment_size_blocks);
  lookup_scalar_field(setting, ENTRY_SIZE_WORDS_FIELD, entry_size_words);

  if (max_segments > MAX_CATALOG_SEGMENTS) {
    throw std::runtime_error(
        "invalid catalog configuration: max_segments exceeds limit");
  }
  if (segment_size_blocks != SEGMENT_SIZE_BLOCKS) {
    throw std::runtime_error(
        "invalid catalog configuration: segment_size_blocks must be equal to "
        "SEGMENT_SIZE_BLOCKS");
  }
  if (entry_size_words != ENTRY_SIZE_WORDS) {
    throw std::runtime_error(
        "invalid catalog configuration: entry_size_word must be equal to "
        "ENTRY_SIZE_WORDS");
  }
}