#pragma once

#include <chrono>
#include <cstddef>
#include <string>

namespace filesystem {

/**
 * @brief Enumeration representing the type of a file.
 */
enum class FileType {
    FREE,       ///< Indicates a temporary or deletable file.
    PERMANENT,  ///< Represents a regular, permanent file.
    BLOCKED     ///< Denotes a file that is locked or in use.
};

/**
 * @brief Represents a file record containing metadata such as name, size, type, and creation timestamp.
 */
class FileRecord final {
   public:
    /**
     * @brief Constructs a FileRecord with the given name, size, and type.
     *        The timestamp is initialized to the current system time.
     * @param name The name of the file.
     * @param size The size of the file in bytes.
     * @param type The type of the file (default: FileType::PERMANENT).
     */
    FileRecord(const std::string& name, size_t size, FileType type = FileType::PERMANENT)
        : type_(type), name_(name), size_(size), timestamp_(std::chrono::system_clock::now()) {}

    /**
     * @brief Sets the filename.
     * @param name The new filename to assign.
     */
    void set_filename(std::string name) noexcept;

    /**
     * @brief Sets the file size.
     * @param size The new size in bytes.
     */
    void set_size(size_t size) noexcept;

    /**
     * @brief Sets the file type.
     * @param type The new FileType to assign.
     */
    void set_type(FileType type) noexcept;

    /**
     * @brief Retrieves the filename.
     * @return The current filename.
     */
    [[nodiscard]] inline std::string get_filename() const noexcept { return name_; }

    /**
     * @brief Retrieves the file size.
     * @return The size in bytes.
     */
    [[nodiscard]] inline size_t get_size() const noexcept { return size_; }

    /**
     * @brief Retrieves the file type.
     * @return The FileType enum value.
     */
    [[nodiscard]] inline FileType get_type() const noexcept { return type_; }

    /**
     * @brief Retrieves the timestamp as a formatted string.
     * @return The timestamp in a human-readable string format (e.g., ISO 8601).
     */
    [[nodiscard]] std::string get_timestamp() const noexcept { return get_timestamp_as_string(); }

   private:
    FileType type_;                                    ///< The type of the file.
    std::string name_;                                 ///< The name of the file.
    size_t size_;                                      ///< The size of the file in bytes.
    std::chrono::system_clock::time_point timestamp_;  ///< Timestamp of record creation/modification.

    /**
     * @brief Converts the internal timestamp to a formatted string.
     * @return The formatted timestamp string.
     */
    [[nodiscard]] std::string get_timestamp_as_string() const noexcept;
};

}  // namespace filesystem