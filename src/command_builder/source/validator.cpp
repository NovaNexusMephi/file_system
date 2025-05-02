#include "command_builder/validator.hpp"

#include "command_builder/validation_schemes.hpp"

ValidationResult Validator::validate(const std::string& command, const nlohmann::json& json) {
    if (validation_schemes.contains(command)) {
        validator_.set_root_schema(validation_schemes.at(command));
        // try {
        //     validator_.validate(json);
        // } catch (...) {
        //     return ValidationResult::INVALID;
        // }
        // return ValidationResult::VALID;
    }
    return ValidationResult::UNKNOWN_COMMAND;
}
