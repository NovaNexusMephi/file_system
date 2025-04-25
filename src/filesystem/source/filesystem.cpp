#include "filesystem/filesystem.hpp"

#include <iostream>

#include "filesystem/catalog.hpp"

namespace filesystem {

Error FileSystem::init(const std::string& volume, const std::string& name, size_t segm, size_t vol,
                       size_t rec) noexcept {
    try {
        info_block_ = new SystemInformation(volume, name);
        catalog_ = new Catalog(segm, vol, rec);
    } catch (std::bad_alloc& e) {
        return Error::NO_MEMORY;
    } catch (...) {
        return Error::ERROR_EXCEPTION;
    }
    return Error::NO_ERROR;
}

std::vector<std::string> FileSystem::dir(bool full) const noexcept {
    if (!full) {
        return catalog_->dir();
    }
    std::vector<std::string> result;
    std::string temp;
    size_t blocked_block = 0;
    result.emplace_back("Volume:" + info_block_->get_volume_name() + ", Owner:" + info_block_->get_owner_name());
    for (const auto& segment : catalog_->get_segments()) {
        for (const auto& record : segment.get_records()) {
            if (record.get_type() == FileType::BLOCKED) {
                blocked_block += record.get_size();
            }
        }
    }
    result.emplace_back("Free blocks:" + std::to_string(catalog_->get_free_space()));
    result.emplace_back("Bad blocks:" + std::to_string(blocked_block));
    auto vec = catalog_->dir();
    for (auto i : vec) {
        result.emplace_back(std::move(i));
    }
    return result;
}

Error FileSystem::vol(const std::string& vol, const std::string& name) noexcept {
    info_block_->set_volume_name(vol);
    info_block_->set_owner_name(name);
    return Error::NO_ERROR;
}

}  // namespace filesystem