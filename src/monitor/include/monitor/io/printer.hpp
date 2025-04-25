#pragma once

#include <string>
namespace monitor::io {

/**
 * @brief An abstract class for printing messages.
 */
class Printer {
   public:
    Printer() = default;
    explicit Printer(const std::string& p_name);

    virtual ~Printer() = default;

    [[nodiscard]] std::string get_name() const noexcept;

    void set_name(const std::string& new_name);

    /**
     * @brief Prints a message.
     * 
     * @param[in] msg The message to print.
     */
    virtual void print(const std::string& msg) = 0;

   protected:
    std::string name_{};
};
}  // namespace monitor::io