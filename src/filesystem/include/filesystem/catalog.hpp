#pragma once

#include <vector>

#include "segment.hpp"

namespace filesystem {

class Catalog final {
   public:
    Catalog(size_t count, size_t counter) : header_(count, counter) { segments_.reserve(count); }

    void create(const std::string& filename, size_t size);

    void remove(const std::string& filename);

    void rename(const std::string& old_filename, const std::string& new_filename);

    void copy(const std::string& filename, const std::string& dist_filename);

    void move(const std::string& filename, const std::string& dist_filename);

    void squeeze();

    void add(const std::string& filename, size_t size);

    [[nodiscard]] std::vector<std::string> dir(bool full = false) const;

    void free() const;

    void vol() const;

    void attr();

    [[nodiscard]] inline const std::vector<Segment>& get_segments() const noexcept { 
        return segments_; 
    }

    [[nodiscard]] inline std::vector<Segment>& get_segments() noexcept { 
        return segments_; 
    }

    inline void set_count(size_t count) {
        header_.count_ = count; 
    }

    inline void setCounter(size_t counter) { 
        header_.counter_ = counter; 
    }

   private:
    [[nodiscard]] std::optional<FileRecord> find_record(const std::string& filename) const;

    struct CatalogHeader {
        size_t count_;
        size_t counter_;

        CatalogHeader(size_t count, size_t counter) : count_(count), counter_(counter) {}
    };

    CatalogHeader header_;
    std::vector<Segment> segments_;
    size_t free_records_;
    size_t free_space_;
};

}  // namespace filesystem