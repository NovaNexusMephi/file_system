#pragma once

#include <string>

namespace filesystem {

class SystemInformation final {
   public:
    SystemInformation() = delete;

    SystemInformation(std::string vol, std::string owner, std::string name)
        : volume_name_(std::move(vol)), owner_name_(std::move(owner)), system_name_(std::move(name)) {}

    inline void set_volume_name(std::string vol) noexcept { 
        volume_name_ = std::move(vol); 
    }

    inline void set_owner_name(std::string owner) noexcept { 
        owner_name_ = std::move(owner); 
    }

    inline void set_system_name(std::string name) noexcept { 
        system_name_ = std::move(name); 
    }

    [[nodiscard]] inline std::string get_volume_name() const noexcept { 
        return volume_name_; 
    }

    [[nodiscard]] inline std::string get_owner_name() const noexcept { 
        return owner_name_; 
    }

    [[nodiscard]] inline std::string get_system_name() const noexcept { 
        return system_name_; 
    }

   private:
    std::string volume_name_;
    std::string owner_name_;
    std::string system_name_;
};

}  // namespace filesystem