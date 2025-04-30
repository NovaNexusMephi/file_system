#include "create_command.hpp"
#include "filesystem/file_record.hpp"

std::string CreateCommand::execute(const nlohmann::json &json) {
    if (!receiver_.get_valid()) {
        return ERROR + ": the file system has not been initialized";
    }

    auto data = json["data"].get<std::vector<std::string>>();

    std::string filename = data[0];
    size_t size = json["options"]["allocate"].get<size_t>();

    if (receiver_.get_catalog().get_files().contains(filename)) {
        return FILE_ALREADY_EXISTS;
    }

    if (size > receiver_.get_catalog().get_free_space()) {
        return NO_FREE_SPACE;
    }
    if (receiver_.get_catalog().get_free_records() == 0) {
        return NO_FREE_RECORDS;
    }
    for (size_t i = 0; i < receiver_.get_catalog().get_count(); ++i) {
        for (size_t j = 0, recsize = receiver_.get_catalog().get_segments()[i].get_counter(); j < recsize; ++j) {
            auto& record = receiver_.get_catalog().get_segments()[i].get_records()[j];
            if (record.get_type() == filesystem::FileType::FREE) {
                size_t sum = 0;
                for (size_t k = j; k < recsize && receiver_.get_catalog().get_segments()[i].get_records()[k].get_type() == filesystem::FileType::FREE; ++k) {
                    sum += receiver_.get_catalog().get_segments()[i].get_records()[k].get_size();
                    if (sum >= size) {
                        receiver_.get_catalog().get_segments()[i].get_records()[j].set_size(size);
                        receiver_.get_catalog().get_files().insert(filename);
                        record.set_type(filesystem::FileType::PERMANENT);
                        receiver_.get_catalog().get_free_space() -= size;
                        --receiver_.get_catalog().get_free_records();
                        for (size_t l = j + 1; l <= k; ++l) {
                            receiver_.get_catalog().get_segments()[i].get_records()[l].set_type(filesystem::FileType::BLOCKED);
                        }
                        receiver_.get_catalog().get_blocked_space() += sum - size;
                        return OK + ": the file has been added";
                    }
                }
            }
        }
    }
    auto& last_segment = receiver_.get_catalog().get_segments()[receiver_.get_catalog().get_busy_segments_count()];
    if (receiver_.get_catalog().get_busy_segments_count() < receiver_.get_catalog().get_count() 
        && size <= receiver_.get_catalog().get_free_direct_space()) {
        if (last_segment.add_record(filename, size)) {
            ++receiver_.get_catalog().get_busy_segments_count();
        }
        receiver_.get_catalog().get_free_direct_space() -= size;
        --receiver_.get_catalog().get_free_records();
        receiver_.get_catalog().get_free_space() -= size;
        receiver_.get_catalog().get_files().insert(filename);
        return OK + ": the file has been added";
    }
    return NO_FREE_SPACE;
}