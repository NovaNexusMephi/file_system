#pragma once

#include <string>

#include "catalog.hpp"
#include "system_information.hpp"

namespace filesystem {

class FileSystem {
   public:
    FileSystem() = default;

    [[nodiscard]] inline const SystemInformation& get_info() const noexcept { return info_block_; }

    [[nodiscard]] inline SystemInformation& get_info() noexcept { return info_block_; }

    inline void set_info(const SystemInformation& info) noexcept { info_block_ = info; }

    [[nodiscard]] inline const Catalog& get_catalog() const noexcept { return catalog_; }

    [[nodiscard]] inline Catalog& get_catalog() noexcept { return catalog_; }

    [[nodiscard]] inline bool get_valid() noexcept {
        return valid_; 
    }

    inline void set_valid(bool flag) noexcept { 
        valid_ = flag; 
    }

    //вот тут хуй знает
    inline void set_catalog(const Catalog& catalog) noexcept {
        catalog_ = catalog;
    }
    
   private:
    SystemInformation info_block_;
    Catalog catalog_;
    bool valid_ = false; //flag
};

}  // namespace filesystem