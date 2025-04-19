#pragma once

#include <string>

namespace filesystem {

class SystemInformation final {
   public:
    SystemInformation() = delete;

    SystemInformation(std::string vol, std::string owner, std::string name)
        : volume_name_(std::move(vol)), owner_name_(std::move(owner)), system_name_(std::move(name)) {}

    inline void setVolumeName(std::string vol) noexcept { volume_name_ = std::move(vol); }

    inline void setOwnerName(std::string owner) noexcept { owner_name_ = std::move(owner); }

    inline void setSystemName(std::string name) noexcept { system_name_ = std::move(name); }

    [[nodiscard]] inline std::string getVolumeName() const noexcept { return volume_name_; }
    [[nodiscard]] inline std::string getOwnerName() const noexcept { return owner_name_; }

    [[nodiscard]] inline std::string getSystemName() const noexcept { return system_name_; }

   private:
    std::string volume_name_;
    std::string owner_name_;
    std::string system_name_;
};

}  // namespace filesystem