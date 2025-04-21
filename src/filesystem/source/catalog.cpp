#include "filesystem/catalog.hpp"

#include <format>

namespace filesystem {

void Catalog::create(const std::string& filename, size_t size) {
    if (size > free_space_) {
        throw std::runtime_error("There is no free space in the catalog");
    }
    if (free_records_ == 0) {
        throw std::runtime_error("There are no free records in the catalog");
    }

    if (header_.counter_ < header_.count_) {
        segments_[header_.counter_].addRecord(filename, size);
    }
    for (auto& segment : segments_) {
        for (const auto& record : segment.getRecords()) {
            if (record.getType() == FileType::FREE) {}
        }
    }
}

void Catalog::remove(const std::string& filename) {
    auto record = findRecord(filename);
    if (record) {
        record->setType(FileType::FREE);
        ++free_records_;
        free_space_ += record->getSize();
        return;
    }
    throw std::runtime_error("File not found");
}

void Catalog::rename(const std::string& old_filename, const std::string& new_filename) {
    auto record = findRecord(old_filename);
    if (record) {
        record->setFilename(new_filename);
        return;
    }
    throw std::runtime_error("File not found");
}

void Catalog::copy(const std::string& filename, const std::string& dist_filename) {
    auto record = findRecord(filename);
    if (record) {
        create(dist_filename, record->getSize());
        return;
    }
    throw std::runtime_error("File not found");
}

void Catalog::move(const std::string& filename, const std::string& dist_filename) {
    auto record = find_record(filename);
    if (record) {
        create(dist_filename, record->get_size());
        remove(filename);
        return;
    }
    throw std::runtime_error("File not found");
}

std::optional<FileRecord> Catalog::find_record(const std::string& filename) const {
    for (const auto& segment : segments_) {
        for (const auto& record : segment.get_records()) {
            if (record.get_type() != FileType::FREE && record.getFilename() == filename) {
                return {record};
            }
        }
    }
    return std::nullopt;
}

std::vector<std::string> Catalog::dir(bool full = false) const {
    std::vector<std::string> result;
    std::string temp;
    for (const auto& segment : segments_) {
        for (const auto& record : segment.getRecords()) {
            if (record.getType() != FileType::FREE) {
                temp = std::format("{} {} {}", record.getTimestamp(), record.getSize(), record.getFilename());
                result.push_back(temp);
            }
        }
    }
    return result;
}

}  // namespace filesystem