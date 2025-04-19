#pragma once

#include <string>

#include "catalog.hpp"
#include "system_information.hpp"

namespace filesystem {

class FileSystem {
public:
    FileSystem();
    bool init_catalog();
    void exit();
    std::string help() const;

    [[nodiscard]] inline SystemInformation getInfo() const noexcept {
        return info_block_;
    }

private:
    SystemInformation info_block_;
    Catalog catalog_;
};

}  // namespace filesystem