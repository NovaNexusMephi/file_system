#pragma once

#include <memory>
#include <nlohmann/json.hpp>

#include "commands/abstract_command.hpp"
#include "validator.hpp"

namespace command_builder {

/**
 * @brief Builds command objects based on JSON input and validation rules.
 */
class CommandBuilder {
   public:
    /**
     * @brief Constructs a CommandBuilder with a reference to the target FileSystem.
     * @param receiver The FileSystem instance to execute commands against.
     */
    explicit CommandBuilder(filesystem::FileSystem& receiver) : receiver_(receiver) {}

    /**
     * @brief Creates a command object from JSON input after validation.
     * @param json Input JSON containing command name, data, and options.
     * @return Unique pointer to the constructed command (nullptr on failure).
     */
    std::unique_ptr<AbstractCommand> build(const nlohmann::json& json);

   private:
    Validator validator_;               ///< Validates command syntax and parameters
    filesystem::FileSystem& receiver_;  ///< Target FileSystem for command execution
};

}  // namespace command_builder