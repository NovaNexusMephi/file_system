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

   private:
    SystemInformation info_block_;
    Catalog catalog_;
};

}  // namespace filesystem