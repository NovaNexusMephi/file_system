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

    bool open(const std::string& filename);

    void close(const std::string& filename);

    void write(const std::string& filename, const std::string& data);  // ?????

    void read(const std::string& filename) const;

    void squeeze();

    void add(const std::string& filename, const std::string& data);  // ?????

    // vol only это вообще что такое
    void dir(bool is_full = false, const std::string& dist_filename = "") const;

    void free() const;

    void vol() const;

    void attr();

    [[nodiscard]] inline const std::vector<Segment>& getSegments() const noexcept { return segments_; }

    [[nodiscard]] inline std::vector<Segment>& getSegments() noexcept { return segments_; }

    inline void setCount(size_t count) { header_.setCount(count); }

    inline void setCounter(size_t counter) { header_.setCounter(counter); }

   private:
    std::optional<FileRecord> findRecord(const std::string& filename) const;

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