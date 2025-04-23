#pragma once

#include <string>

#include "catalog.hpp"
#include "system_information.hpp"

namespace filesystem {

class FileSystem {
   public:
    FileSystem() = default;

    Error init(const std::string& volume, const std::string& name = "", size_t segm = 0, size_t vol = 0,
               size_t rec = 0) noexcept;

    std::vector<std::string> dir(bool full = false) const noexcept;

    Error vol(const std::string& vol, const std::string& name = "") noexcept;

    void exit();

    std::string help() const;

    [[nodiscard]] inline const SystemInformation* get_info() const noexcept { return info_block_; }

    [[nodiscard]] inline SystemInformation* get_info() noexcept { return info_block_; }

    inline void set_info(const SystemInformation& info) noexcept { *info_block_ = info; }

    [[nodiscard]] inline const Catalog* get_catalog() const noexcept { return catalog_; }

    [[nodiscard]] inline Catalog* get_catalog() noexcept { return catalog_; }

    [[nodiscard]] inline Error create(const std::string& filename, size_t size) noexcept {
        return catalog_->create(filename, size);
    }

    [[nodiscard]] inline Error remove(const std::string& filename) noexcept { return catalog_->remove(filename); }

    [[nodiscard]] inline Error rename(const std::string& old_filename, const std::string& new_filename) noexcept {
        return catalog_->rename(old_filename, new_filename);
    }

    [[nodiscard]] inline Error copy(const std::string& filename, const std::string& dist_filename) noexcept {
        return catalog_->copy(filename, dist_filename);
    }

    [[nodiscard]] inline Error move(const std::string& filename, const std::string& dist_filename) noexcept {
        return catalog_->move(filename, dist_filename);
    }

    [[nodiscard]] inline Error squeeze() const noexcept { return catalog_->squeeze(); }

    [[nodiscard]] inline Error add(const std::string& filename, size_t size) const noexcept {
        return catalog_->add(filename, size);
    }

    [[nodiscard]] inline Error free() const noexcept { return catalog_->free(); }

   private:
    SystemInformation* info_block_ = nullptr;
    Catalog* catalog_ = nullptr;
};

}  // namespace filesystem