#include "filesystem/catalog.hpp"

#include <algorithm>
#include <format>
#include <iostream>

#include "filesystem/file_record.hpp"

namespace filesystem {

Error Catalog::create(const std::string& filename, size_t size) noexcept {
    if (size > header_.free_space_) {
        return Error::NO_FREE_SPACE;
    }
    if (header_.free_records_ == 0) {
        return Error::NO_FREE_RECORDS;
    }
    if (files_.contains(filename)) {
        return Error::FILE_ALREADY_EXISTS;
    }
    for (size_t i = 0; i < header_.count_; ++i) {
        for (size_t j = 0, recsize = segments_[i].get_counter(); j < recsize; ++j) {
            auto& record = segments_[i].get_records()[j];
            if (record.get_type() == FileType::FREE) {
                size_t sum = 0;
                for (size_t k = j; k < recsize && segments_[i].get_records()[k].get_type() == FileType::FREE; ++k) {
                    sum += segments_[i].get_records()[k].get_size();
                    if (sum >= size) {
                        segments_[i].get_records()[j].set_size(size);
                        files_.insert(filename);
                        record.set_type(FileType::PERMANENT);
                        header_.free_space_ -= size;
                        --header_.free_records_;
                        for (size_t l = j + 1; l <= k; ++l) {
                            segments_[i].get_records()[l].set_type(FileType::BLOCKED);
                        }
                        header_.blocked_space_ += sum - size;
                        return Error::NO_ERROR;
                    }
                }
            }
        }
    }
    auto& last_segment = segments_[header_.counter_];
    if (header_.counter_ < header_.count_ && size <= header_.free_direct_space_) {
        if (last_segment.add_record(filename, size)) {
            ++header_.counter_;
        }
        header_.free_direct_space_ -= size;
        --header_.free_records_;
        header_.free_space_ -= size;
        files_.insert(filename);
        return Error::NO_ERROR;
    }
    return Error::NO_FREE_SPACE;
}

Error Catalog::remove(const std::string& filename) noexcept {
    auto record = find_record(filename);
    if (record) {
        record->set_type(FileType::FREE);
        ++header_.free_records_;
        size_t size = record->get_size();
        header_.free_space_ += size;
        files_.erase(filename);
        auto* last_segment = &segments_[header_.counter_];
        if (last_segment->get_counter() == 0) {
            last_segment = &segments_[header_.counter_ - 1];
        }
        if (last_segment->get_records().back().get_type() == FileType::FREE) {
            last_segment->remove_record();
            header_.free_direct_space_ += size;
        }
        return Error::NO_ERROR;
    }
    return Error::FILE_NOT_FOUND;
}

Error Catalog::rename(const std::string& old_filename, const std::string& new_filename) noexcept {
    if (old_filename == new_filename) {
        return Error::NO_ERROR;
    }
    if (files_.contains(new_filename)) {
        return Error::FILE_ALREADY_EXISTS;
    }
    auto record = find_record(old_filename);
    if (record) {
        record->set_filename(new_filename);
        files_.erase(old_filename);
        files_.insert(new_filename);
        return Error::NO_ERROR;
    }
    return Error::FILE_NOT_FOUND;
}

Error Catalog::copy(const std::string& filename, const std::string& dist_filename) noexcept {
    if (files_.contains(dist_filename)) {
        return Error::FILE_ALREADY_EXISTS;
    }
    auto record = find_record(filename);
    if (record) {
        return create(dist_filename, record->get_size());
    }
    return Error::FILE_NOT_FOUND;
}

Error Catalog::move(const std::string& filename, const std::string& dist_filename) noexcept {
    if (files_.contains(dist_filename)) {
        return Error::FILE_ALREADY_EXISTS;
    }
    auto record = find_record(filename);
    if (record) {
        create(dist_filename, record->get_size());
        remove(filename);
        return Error::NO_ERROR;
    }
    return Error::FILE_NOT_FOUND;
}

Error Catalog::add(const std::string& filename, size_t new_size) noexcept {
    auto record = find_record(filename);
    if (record) {
        auto size = record->get_size();
        if (new_size > header_.free_space_) {
            return Error::NO_FREE_SPACE;
        }
        record->set_size(size + new_size);
        header_.free_space_ -= new_size;
        return Error::NO_ERROR;
    }
    return Error::FILE_NOT_FOUND;
}

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

std::vector<std::string> Catalog::dir() const noexcept {
    std::vector<std::string> result;
    std::string temp;
    for (const auto& segment : segments_) {
        for (const auto& record : segment.get_records()) {
            if (record.get_type() != FileType::FREE && record.get_type() != FileType::BLOCKED) {
                temp = std::format("{} {} Blocks {}", record.get_filename(), record.get_size(), record.get_timestamp());
                result.emplace_back(std::move(temp));
            }
        }
    }
    return result;
}

Error Catalog::squeeze() {
    if (header_.free_space_ == 0) {
        return Error::NO_ERROR;
    }
    size_t records_count = segments_[0].get_size();
    std::vector<Segment> new_segments(header_.count_, Segment(records_count));
    size_t k = 0;
    for (auto& segment : segments_) {
        for (auto& record : segment.get_records()) {
            if (record.get_type() == FileType::PERMANENT) {
                if (new_segments[k].add_record(record.get_filename(), record.get_size())) {
                    ++k;
                }
            }
        }
    }
    header_.counter_ = k;
    header_.free_records_ = (header_.count_ - k) * records_count - new_segments[k].get_counter();
    header_.free_direct_space_ = header_.free_space_;
    header_.blocked_space_ = 0;
    segments_.swap(new_segments);
    return Error::NO_ERROR;
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

std::vector<std::string> Catalog::sort(bool by_name, bool by_ext, bool by_date, bool by_size,
                                       bool inverse) const noexcept {
    std::vector<std::string> result = dir();
    if (by_name) {
        std::sort(result.begin(), result.end(), name_compare);
    }
    if (by_ext) {
        std::sort(result.begin(), result.end(), extension_compare);
    }
    if (by_date) {
        std::sort(result.begin(), result.end(), date_compare);
    }
    if (by_size) {
        std::sort(result.begin(), result.end(), size_compare);
    }
    if (inverse) {
        std::reverse(result.begin(), result.end());
    }
    return result;
}

void Catalog::print_catalog() const noexcept {
    for (const auto& segment : segments_) {
        size_t total_size = segment.get_size();
        size_t used_records = segment.get_counter();

        // Сначала выводим существующие записи
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

        // Выводим [EMPTY] для оставшихся неиспользованных записей в сегменте
        for (size_t i = used_records; i < total_size; ++i) {
            std::cout << "[EMPTY]";
        }
    }
    std::cout << std::endl;
}

}  // namespace filesystem