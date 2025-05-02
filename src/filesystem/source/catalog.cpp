#include "filesystem/catalog.hpp"

#include <algorithm>
#include <format>
#include <iostream>

#include "filesystem/file_record.hpp"

namespace filesystem {

FileRecord* Catalog::find_record(const std::string& filename) noexcept {
    for (auto& segment : segments_) {
        for (auto& record : segment.get_records()) {
            if (record.get_type() != FileType::FREE && record.get_type() != FileType::BLOCKED &&
                record.get_filename() == filename) {
                return &record;
            }
        }
    }
    return nullptr;
}

// std::vector<std::string> Catalog::sort(bool by_name, bool by_ext, bool by_date, bool by_size,
//                                        bool inverse) const noexcept {
//     std::vector<std::string> result;  // = dir();
//     if (by_name) {
//         std::sort(result.begin(), result.end(), name_compare);
//     }
//     if (by_ext) {
//         std::sort(result.begin(), result.end(), extension_compare);
//     }
//     if (by_date) {
//         std::sort(result.begin(), result.end(), date_compare);
//     }
//     if (by_size) {
//         std::sort(result.begin(), result.end(), size_compare);
//     }
//     if (inverse) {
//         std::reverse(result.begin(), result.end());
//     }
//     return result;
// }

void Catalog::print_catalog() const noexcept {
    for (const auto& segment : segments_) {
        size_t total_size = segment.get_size();
        size_t used_records = segment.get_counter();
        for (size_t i = 0; i < used_records; ++i) {
            const auto& record = segment.get_records()[i];
            if (record.get_type() == FileType::FREE) {
                std::cout << "[FREE]";
            } else if (record.get_type() == FileType::BLOCKED) {
                std::cout << "[BLOCKED]";
            } else {
                std::cout << "[PERMANENT]";
            }
        }
        for (size_t i = used_records; i < total_size; ++i) {
            std::cout << "[EMPTY]";
        }
    }
    std::cout << std::endl;
}

}  // namespace filesystem