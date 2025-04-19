#pragma once

#include <vector>

#include "segment.hpp"

namespace filesystem {

class Catalog final {
   public:
    Catalog(size_t count, size_t counter) : header_(count, counter) { segments_.reserve(count); }

    void addFile(const std::string& filename, const std::string& content);

    void removeFile(const std::string& filename);

    void listFiles() const;

    [[nodiscard]] inline const std::vector<Segment>& getSegments() const noexcept { return segments_; }

    [[nodiscard]] inline std::vector<Segment>& getSegments() noexcept { return segments_; }

    inline void setCount(size_t count) { header_.setCount(count); }

    inline void setCounter(size_t counter) { header_.setCounter(counter); }

   private:
    struct CatalogHeader {
        size_t count_;
        size_t counter_;

        CatalogHeader(size_t count, size_t counter) : count_(count), counter_(counter) {}
    };

    CatalogHeader header_;
    std::vector<Segment> segments_;
};

}  // namespace filesystem