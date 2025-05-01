#include "command_builder/validator.hpp"

#include "command_builder/validation_schemes.hpp"

ValidationResult Validator::validate(const std::string& command) {
    if (validation_schemes.contains(command)) {
        validator_.set_root_schema(validation_schemes.at(command));
        try {
            validator_.validate(command);
        } catch (...) {
            return ValidationResult::INVALID;
        }
        return ValidationResult::VALID;
    }
    return ValidationResult::UNKNOWN_COMMAND;
}
