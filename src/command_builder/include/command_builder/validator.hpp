#pragma once

#include <nlohmann/json-schema.hpp>
#include <string>

enum class ValidationResult { VALID, INVALID, UNKNOWN_COMMAND };

class Validator {
   public:
    ValidationResult validate(const std::string& command, const nlohmann::json& json);

   private:
    nlohmann::json_schema::json_validator validator_;
};
