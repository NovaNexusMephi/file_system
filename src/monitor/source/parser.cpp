#include "monitor/parser.hpp"
#include <nlohmann/json_fwd.hpp>
#include <optional>
#include <string>
#include <utility>
#include <vector>
using monitor::Parser;

std::optional<nlohmann::json> Parser::parse(const std::string &cmd) noexcept {
  auto tokens = split(cmd, " ;\t\n\r");
  if (tokens.empty())
    return std::nullopt;

  std::vector<std::string> arg_tokens{};
  std::vector<std::string> opt_tokens{};

  for (std::size_t i = 0; i < tokens.size(); ++i) {
    auto token = tokens[i];
    if (token.starts_with('/')) {
      opt_tokens.push_back(token);
    } else {
      arg_tokens.push_back(token);
    }
  }

  if (arg_tokens.empty())
    return std::nullopt;

  nlohmann::json parsed_command = R"({})"_json;

  auto [name, params] = make_argv(arg_tokens);
  parsed_command["name"] = name;
  parsed_command["data"] = params;
  parsed_command["options"] = make_opt_obj(opt_tokens);

  return parsed_command;
}

std::vector<std::string> Parser::split(const std::string &str,
                                       const std::string &delims) noexcept {
  std::vector<std::string> tokens{};
  size_t start = 0, end = 0;

  while ((end = str.find_first_of(delims, start)) != std::string::npos) {
    if (end > start) {
      tokens.emplace_back(str.substr(start, end - start));
    }
    start = end + 1;
  }

  if (start < str.size()) {
    tokens.emplace_back(str.substr(start));
  }

  return tokens;
}

std::pair<std::string, std::string>
Parser::parse_opt(const std::string &opt_token) noexcept {
  if (!opt_token.starts_with('/'))
    return {"", ""};
  const size_t delim_pos = opt_token.find_first_of(":");
  if (delim_pos == std::string::npos)
    return {opt_token.substr(1), ""};
  return {opt_token.substr(1, delim_pos - 1), opt_token.substr(delim_pos + 1)};
}

nlohmann::json
Parser::make_opt_obj(const std::vector<std::string> &opt_tokens) noexcept {
  nlohmann::json options = R"({})"_json;
  for (auto &opt_token : opt_tokens) {
    auto [key, value] = parse_opt(opt_token);
    if (key.empty())
      continue;

    if (value.empty()) {
      options[key] = true;
    } else if (std::all_of(value.begin(), value.end(), ::isdigit)) {
      options[key] = std::stoi(value);
    } else {
      options[key] = value;
    }
  }
  return options;
}

std::pair<std::string, std::vector<std::string>>
Parser::make_argv(const std::vector<std::string> &arg_tokens) noexcept {
  std::string name_token = arg_tokens[0];
  std::vector<std::string> param_tokens{};
  for (std::size_t i = 1; i < arg_tokens.size(); ++i) {
    param_tokens.push_back(arg_tokens[i]);
  }
  return {name_token, param_tokens};
}