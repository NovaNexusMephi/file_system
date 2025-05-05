#pragma once

#include <string>

namespace filesystem {

/**
 * @brief Stores metadata and configuration information for the file system.
 */
class SystemInformation final {
   public:
    /**
     * @brief Default constructor. Initializes empty volume and owner names.
     */
    SystemInformation() = default;

    /**
     * @brief Constructs a SystemInformation object with specified volume and owner.
     * @param vol Name of the storage volume (moved into object).
     * @param owner Name of the volume owner (moved into object).
     */
    SystemInformation(std::string vol, std::string owner)
        : volume_name_(std::move(vol)), owner_name_(std::move(owner)) {}

    /**
     * @brief Sets the volume name using move semantics.
     * @param vol New volume name to assign.
     */
    inline void set_volume_name(std::string vol) noexcept { volume_name_ = std::move(vol); }

    /**
     * @brief Sets the owner name using move semantics.
     * @param owner New owner name to assign.
     */
    inline void set_owner_name(std::string owner) noexcept { owner_name_ = std::move(owner); }

    /**
     * @brief Retrieves the current volume name.
     * @return Reference to the volume name string.
     */
    [[nodiscard]] inline std::string get_volume_name() const noexcept { return volume_name_; }

    /**
     * @brief Retrieves the current owner name.
     * @return Reference to the owner name string.
     */
    [[nodiscard]] inline std::string get_owner_name() const noexcept { return owner_name_; }

   private:
    std::string volume_name_;  ///< Name of the storage volume
    std::string owner_name_;   ///< Name of the volume owner
};

}  // namespace filesystem