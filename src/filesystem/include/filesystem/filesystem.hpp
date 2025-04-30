#pragma once

#include <string>

#include "catalog.hpp"
#include "system_information.hpp"

namespace filesystem {

class FileSystem {
   public:
    FileSystem() = default;

    [[nodiscard]] inline const SystemInformation* get_info() const noexcept { return info_block_; }

    [[nodiscard]] inline SystemInformation* get_info() noexcept { return info_block_; }

    inline void set_info(const SystemInformation& info) noexcept { *info_block_ = info; }

    [[nodiscard]] inline const Catalog* get_catalog() const noexcept { return catalog_; }

    [[nodiscard]] inline Catalog* get_catalog() noexcept { return catalog_; }
   
   private:
    SystemInformation* info_block_ = nullptr;
    Catalog* catalog_ = nullptr;
};

}  // namespace filesystem