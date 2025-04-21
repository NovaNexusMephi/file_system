#include "filesystem/catalog.hpp"

#include <format>
#include <iostream>

#include "filesystem/file_record.hpp"

namespace filesystem {

Error Catalog::create(const std::string& filename, size_t size) {
    if (size > header_.free_space_) {
        return Error::NO_FREE_SPACE;
    }
    if (header_.free_records_ == 0) {
        return Error::NO_FREE_RECORDS;
    }
    if (files_.contains(filename)) {
        return Error::FILE_ALREADY_EXISTS;
    }
    for (size_t i = 0, segsize = segments_.size(); i < segsize; ++i) {
        for (size_t j = 0, recsize = segments_[i].get_records().size(); j < recsize; ++j) {
            FileRecord record = segments_[i].get_records()[j];
            std::cout << "i = " << i << ", j = " << j << std::endl;
            if (record.get_type() == FileType::FREE) {
                std::cout << "record.get_type() == FileType::FREE" << std::endl;
                size_t sum = record.get_size();
                for (size_t k = j + 1; k < recsize && segments_[i].get_records()[k].get_type() == FileType::FREE; ++k) {
                    sum += segments_[i].get_records()[k].get_size();
                    std::cout << "sum = " << sum << std::endl;
                    if (sum >= size) {
                        std::cout << "sum >= size" << std::endl;
                        segments_[i].get_records()[j].set_size(size);
                        files_.insert(filename);
                        header_.free_space_ -= size;
                        --header_.free_records_;
                        for (size_t l = j + 1; l <= k; ++l) {
                            segments_[i].get_records()[l].set_type(FileType::BLOCKED);
                        }
                        std::cout << "for, free space = " << header_.free_space_ << std::endl;
                        return Error::NO_ERROR;
                    }
                }
            }
        }
    }
    if (header_.counter_ < header_.count_) {
        if (segments_[header_.counter_].add_record(filename, size)) {
            ++header_.counter_;
        }
        --header_.free_records_;
        header_.free_space_ -= size;
        std::cout << "if, free space = " << header_.free_space_ << std::endl;
        return Error::NO_ERROR;
    }
    return Error::NO_FREE_SPACE;
}

Error Catalog::remove(const std::string& filename) {
    auto record = find_record(filename);
    if (record) {
        record->set_type(FileType::FREE);
        ++header_.free_records_;
        header_.free_space_ += record->get_size();
        files_.erase(filename);
        return Error::NO_ERROR;
    }
    return Error::FILE_NOT_FOUND;
}

Error Catalog::rename(const std::string& old_filename, const std::string& new_filename) {
    auto record = find_record(old_filename);
    if (record) {
        record->set_filename(new_filename);
        return Error::NO_ERROR;
    }
    return Error::FILE_NOT_FOUND;
}

Error Catalog::copy(const std::string& filename, const std::string& dist_filename) {
    auto record = find_record(filename);
    if (record) {
        create(dist_filename, record->get_size());
        return Error::NO_ERROR;
    }
    return Error::FILE_NOT_FOUND;
}

Error Catalog::move(const std::string& filename, const std::string& dist_filename) {
    auto record = find_record(filename);
    if (record) {
        create(dist_filename, record->get_size());
        remove(filename);
        return Error::NO_ERROR;
    }
    return Error::FILE_NOT_FOUND;
}

std::optional<FileRecord> Catalog::find_record(const std::string& filename) const {
    for (const auto& segment : segments_) {
        for (const auto& record : segment.get_records()) {
            if (record.get_type() != FileType::FREE && record.get_type() != FileType::BLOCKED &&
                record.get_filename() == filename) {
                return {record};
            }
        }
    }
    return std::nullopt;
}

std::vector<std::string> Catalog::dir(bool full) const {
    std::vector<std::string> result;
    std::string temp;
    for (const auto& segment : segments_) {
        for (const auto& record : segment.get_records()) {
            if (record.get_type() != FileType::FREE) {
                temp = std::format("{} {} {}", record.get_timestamp(), record.get_size(), record.get_filename());
                result.push_back(temp);
            }
        }
    }
    return result;
}

}  // namespace filesystem