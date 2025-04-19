#pragma once

#include <vector>

#include "segment.hpp"

namespace filesystem {

class Catalog final {
public:
    Catalog();
    void addFile(const std::string& filename, const std::string& content);
    void removeFile(const std::string& filename);
    void listFiles() const;

private:
    struct CatalogHeader {
        size_t number_;
        size_t counter_;
    };

    CatalogHeader header_;
    std::vector<Segment> segments_;
};

}  // namespace filesystem