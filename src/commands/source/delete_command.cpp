#include "commands/delete_command.hpp"

#include "filesystem/file_record.hpp"

std::string DeleteCommand::execute(const nlohmann::json& json) {
    if (!receiver_.is_valid()) {
        return ERROR + ": the file system has not been initialized";
    }

    auto data = json["data"].get<std::vector<std::string>>();
    std::string filename = data[0];

    auto& catalog = receiver_.get_catalog();
    auto* record = catalog.find_record(filename);

    if (record) {
        record->set_type(filesystem::FileType::FREE);
        ++catalog.get_free_records();
        size_t size = record->get_size();
        catalog.get_free_space() += size;
        catalog.get_files().erase(filename);
        auto* last_segment = &catalog.get_segments()[catalog.get_busy_segments_count()];
        if (last_segment->get_counter() == 0) {
            last_segment = &catalog.get_segments()[catalog.get_busy_segments_count() - 1];
        }
        if (last_segment->get_records().back().get_type() == filesystem::FileType::FREE) {
            last_segment->remove_record();
            catalog.get_free_direct_space() += size;
        }
        return OK + ": the file has been removed";
    }
    return FILE_NOT_FOUND;
}
