#pragma once

#include <optional>
#include <unordered_set>
#include <vector>

#include "segment.hpp"

namespace filesystem {

enum class Error {
    NO_ERROR,
    NO_FREE_RECORDS,
    NO_FREE_SPACE,
    FILE_NOT_FOUND,
    FILE_ALREADY_EXISTS,
};

class Catalog final {
   public:
    Catalog(size_t count, size_t records_count, size_t volume_size)
        : header_(count, 0, records_count * count, volume_size), segments_(count, Segment(0, 0, records_count)) {}

    Error create(const std::string& filename, size_t size);

    Error remove(const std::string& filename);

    Error rename(const std::string& old_filename, const std::string& new_filename);

    Error copy(const std::string& filename, const std::string& dist_filename);

    Error move(const std::string& filename, const std::string& dist_filename);

    Error squeeze();

    Error add(const std::string& filename, size_t size);

    Error free() const;

    Error vol() const;

    Error attr();

    [[nodiscard]] std::vector<std::string> dir(bool full = false) const;

    [[nodiscard]] inline const std::vector<Segment>& get_segments() const noexcept { return segments_; }

    [[nodiscard]] inline std::vector<Segment>& get_segments() noexcept { return segments_; }

    inline void set_count(size_t count) { header_.count_ = count; }

    inline void setCounter(size_t counter) { header_.counter_ = counter; }

   private:
    [[nodiscard]]FileRecord* find_record(const std::string& filename);

    struct CatalogHeader {
        size_t count_;
        size_t counter_;
        size_t free_records_;
        size_t free_space_;

        CatalogHeader(size_t count, size_t counter, size_t records_count, size_t volume_size)
            : count_(count), counter_(counter), free_records_(records_count), free_space_(volume_size) {}
    };

    CatalogHeader header_;
    std::vector<Segment> segments_;
    std::unordered_set<std::string> files_;
};

}  // namespace filesystem