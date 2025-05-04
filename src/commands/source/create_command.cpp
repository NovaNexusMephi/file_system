#include "commands/create_command.hpp"

#include "filesystem/file_record.hpp"

std::string CreateCommand::execute() {
    if (!receiver_.is_valid()) {
        return ERROR + ": the file system has not been initialized";
    }

    auto& catalog = receiver_.get_catalog();

    if (catalog.get_files().contains(filename_)) {
        return FILE_ALREADY_EXISTS;
    }

    if (size_ > catalog.get_free_space()) {
        return NO_FREE_SPACE;
    }
    if (catalog.get_free_records() == 0) {
        return NO_FREE_RECORDS;
    }
    for (size_t i = 0; i < catalog.get_count(); ++i) {
        for (size_t j = 0, recsize = catalog.get_segments()[i].get_counter(); j < recsize; ++j) {
            auto& record = catalog.get_segments()[i].get_records()[j];
            if (record.get_type() == filesystem::FileType::FREE) {
                size_t sum = 0;
                for (size_t k = j;
                     k < recsize && catalog.get_segments()[i].get_records()[k].get_type() == filesystem::FileType::FREE;
                     ++k) {
                    sum += catalog.get_segments()[i].get_records()[k].get_size();
                    if (sum >= size_) {
                        catalog.get_segments()[i].get_records()[j].set_size(size_);
                        catalog.get_files().insert(filename_);
                        record.set_type(filesystem::FileType::PERMANENT);
                        catalog.get_free_space() -= size_;
                        --catalog.get_free_records();
                        for (size_t l = j + 1; l <= k; ++l) {
                            catalog.get_segments()[i].get_records()[l].set_type(filesystem::FileType::BLOCKED);
                        }
                        catalog.get_blocked_space() += sum - size_;
                        return OK + std::format(": the {} has been added", filename_);
                    }
                }
            }
        }
    }
    auto& last_segment = catalog.get_segments()[catalog.get_busy_segments_count()];
    if (catalog.get_busy_segments_count() < catalog.get_count() && size_ <= catalog.get_free_direct_space()) {
        if (last_segment.add_record(filename_, size_)) {
            ++catalog.get_busy_segments_count();
        }
        catalog.get_free_direct_space() -= size_;
        --catalog.get_free_records();
        catalog.get_free_space() -= size_;
        catalog.get_files().insert(filename_);
        return OK + std::format(": the {} has been added", filename_);
    }
    return NO_FREE_SPACE;
}