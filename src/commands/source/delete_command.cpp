#include "commands/delete_command.hpp"

#include "filesystem/file_record.hpp"

std::string DeleteCommand::execute() {
    if (!receiver_.is_valid()) {
        return ERROR + ": the file system has not been initialized";
    }

    auto& catalog = receiver_.get_catalog();
    auto* record = catalog.find_record(filename_);

    if (record) {
        record->set_type(filesystem::FileType::FREE);
        ++catalog.get_free_records();
        size_t size = record->get_size();
        catalog.get_free_space() += size;
        catalog.get_files().erase(filename_);
        size_t last_segment_index = catalog.get_busy_segments_count();
        filesystem::Segment* last_segment;
        if (last_segment_index == catalog.get_count()) {
            last_segment = &catalog.get_segments().back();
            if (last_segment->get_records().back().get_type() == filesystem::FileType::FREE) {
                last_segment->remove_record();
                catalog.get_free_direct_space() += size;
                --catalog.get_busy_segments_count();
            }

        } else {
            if (catalog.get_segments()[last_segment_index].get_counter() == 0) {
                last_segment = &catalog.get_segments()[last_segment_index - 1];
                if (last_segment->get_records().back().get_type() == filesystem::FileType::FREE) {
                    last_segment->remove_record();
                    catalog.get_free_direct_space() += size;
                    --catalog.get_busy_segments_count();
                }
            } else {
                last_segment = &catalog.get_segments()[last_segment_index];
                if (last_segment->get_records().back().get_type() == filesystem::FileType::FREE) {
                    last_segment->remove_record();
                    catalog.get_free_direct_space() += size;
                }
            }
        }
        return OK + std::format(": the {} has been removed", filename_);
    }
    return FILE_NOT_FOUND;
}