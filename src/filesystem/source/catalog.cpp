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

auto get_extension = [](const std::string& line) -> std::string {
    size_t pos = line.find_last_of('.');
    return (pos == std::string::npos) ? "" : line.substr(pos);
};

auto extract_date = [](const std::string& line) -> std::string {
    size_t pos = line.rfind(' ');
    return (pos == std::string::npos) ? "" : line.substr(pos + 1);
};

auto extract_size = [](const std::string& line) -> size_t {
    size_t pos = line.find(" Blocks");
    if (pos == std::string::npos)
        return 0;
    std::string size_str = line.substr(0, pos);
    size_t space_pos = size_str.find_last_of(' ');
    return std::stoul(size_str.substr(space_pos + 1));
};

auto name_compare = [](const std::string& a, const std::string& b) -> bool {
    return a < b;
};

auto extension_compare = [](const std::string& a, const std::string& b) -> bool {
    return get_extension(a) < get_extension(b);
};

auto date_compare = [](const std::string& a, const std::string& b) -> bool {
    return extract_date(a) < extract_date(b);
};

auto size_compare = [](const std::string& a, const std::string& b) -> bool {
    return extract_size(a) < extract_size(b);
};

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