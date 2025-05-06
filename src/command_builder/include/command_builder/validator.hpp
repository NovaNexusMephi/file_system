#pragma once

#include <nlohmann/json-schema.hpp>
#include <string>

namespace command_builder {

/**
 * @brief Enum representing the result of command validation.
 */
enum class ValidationResult {
    VALID,           ///< Command is valid and matches schema.
    INVALID,         ///< Command structure is invalid.
    UNKNOWN_COMMAND  ///< Command name not recognized.
};

/**
 * @brief Validates JSON commands against predefined schemas.
 */
class Validator {
   public:
    /**
     * @brief Validates a command's JSON structure against its schema.
     * 
     * @param command Name of the command to validate.
     * @param json Input JSON to validate.
     * @return ValidationResult indicating validation outcome.
     */
    ValidationResult validate(const std::string& command, const nlohmann::json& json);

   private:
    nlohmann::json_schema::json_validator validator_;  ///< JSON schema validator instance.
};

}  // namespace command_builder