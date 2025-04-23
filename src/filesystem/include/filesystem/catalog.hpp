#pragma once

#include <optional>
#include <unordered_set>
#include <vector>

#include "filesystem/system_information.hpp"
#include "segment.hpp"
#include "system_information.hpp"

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
        : header_(count, 0, records_count * count, volume_size), segments_(count, Segment(0, 0, records_count))
        {}

    [[nodiscard]] Error create(const std::string& filename, size_t size) noexcept;

    [[nodiscard]] Error remove(const std::string& filename) noexcept;

    [[nodiscard]] Error rename(const std::string& old_filename, const std::string& new_filename) noexcept;

    [[nodiscard]] Error copy(const std::string& filename, const std::string& dist_filename) noexcept;

    [[nodiscard]] Error move(const std::string& filename, const std::string& dist_filename) noexcept;

    [[nodiscard]] Error squeeze();

    [[nodiscard]] Error add(const std::string& filename, size_t size) noexcept;

    [[nodiscard]] Error free() const noexcept;

    [[nodiscard]] Error vol() const;

    [[nodiscard]] Error attr();

    [[nodiscard]] std::vector<std::string> dir() const noexcept;

    [[nodiscard]] std::vector<std::string> sort(bool by_name = false, bool by_ext = false, bool by_date = false, 
                bool by_size = false, bool inverse = false) const noexcept;

    [[nodiscard]] inline const std::vector<Segment>& get_segments() const noexcept { return segments_; }

    [[nodiscard]] inline std::vector<Segment>& get_segments() noexcept { return segments_; }

    inline void set_count(size_t count) noexcept { header_.count_ = count; }

    inline void set_counter(size_t counter) noexcept { header_.counter_ = counter; }

   private:
    [[nodiscard]] FileRecord* find_record(const std::string& filename) noexcept;

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