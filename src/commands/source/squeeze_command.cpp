#include "commands/squeeze_command.hpp"
#include "filesystem/file_record.hpp"
#include "filesystem/segment.hpp"

std::string SqueezeCommand::execute(const nlohmann::json& json) {
    if (!receiver_.is_valid()) {
        return ERROR + ": the file system has not been initialized";
    }
    auto& catalog = receiver_.get_catalog();
    if(catalog.get_free_space() == 0) {
        return OK + ": fragmentation was completed successfully";
    }
    auto& segments = catalog.get_segments();
    size_t records_count = segments[0].get_size(), k = 0;
    std::vector<filesystem::Segment> new_segments(catalog.get_count(), filesystem::Segment(records_count));
    for(auto& segment : segments) {
        for(auto& record : segment.get_records()) {
            if (record.get_type() == filesystem::FileType::PERMANENT) {
                if (new_segments[k].add_record(record.get_filename(), record.get_size())) {
                    ++k;
                }
            }
        }
    }
    catalog.get_busy_segments_count() = k;
    catalog.get_free_records() = (catalog.get_count() - k) * records_count - new_segments[k].get_counter();
    catalog.get_free_direct_space() = catalog.get_free_space();
    catalog.get_blocked_space() = 0;
    segments.swap(new_segments);
    return OK + ": fragmentation was completed successfully";
}
