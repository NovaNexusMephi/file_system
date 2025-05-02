#include "commands/create_command.hpp"

#include "filesystem/file_record.hpp"

std::string CreateCommand::execute(const nlohmann::json& json) {
    if (!receiver_.is_valid()) {
        return ERROR + ": the file system has not been initialized";
    }

    auto data = json["data"].get<std::vector<std::string>>();

    size_t size = json["options"]["allocate"].get<size_t>();
    std::string filename = data[0];

    auto& catalog = receiver_.get_catalog();

    if (catalog.get_files().contains(filename)) {
        return FILE_ALREADY_EXISTS;
    }

    if (size > catalog.get_free_space()) {
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
                    if (sum >= size) {
                        catalog.get_segments()[i].get_records()[j].set_size(size);
                        catalog.get_files().insert(filename);
                        record.set_type(filesystem::FileType::PERMANENT);
                        catalog.get_free_space() -= size;
                        --catalog.get_free_records();
                        for (size_t l = j + 1; l <= k; ++l) {
                            catalog.get_segments()[i].get_records()[l].set_type(filesystem::FileType::BLOCKED);
                        }
                        catalog.get_blocked_space() += sum - size;
                        return OK + ": the file has been added";
                    }
                }
            }
        }
    }
    auto& last_segment = catalog.get_segments()[catalog.get_busy_segments_count()];
    if (catalog.get_busy_segments_count() < catalog.get_count() && size <= catalog.get_free_direct_space()) {
        if (last_segment.add_record(filename, size)) {
            ++catalog.get_busy_segments_count();
        }
        catalog.get_free_direct_space() -= size;
        --catalog.get_free_records();
        catalog.get_free_space() -= size;
        catalog.get_files().insert(filename);
        return OK + ": the file has been added";
    }
    return NO_FREE_SPACE;
}