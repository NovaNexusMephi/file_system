#pragma once

#include <string>

namespace filesystem {

class SystemInformation {
   public:
    SystemInformation();
    std::string getSystemName() const;

   private:
    std::string volume_name_;
    std::string owner_name_;
    std::string system_name_;
};

}  // namespace filesystem