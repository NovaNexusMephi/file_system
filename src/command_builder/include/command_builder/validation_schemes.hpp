#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

static const nlohmann::json dir_scheme = R"(
{
     "$schema": "http://json-schema.org/draft-07/schema#",
     "title": "dir",
     "type": "object",
     "properties": {
        "name": {
            "type": "string"
        },
        "data": {
            "type": "array",
            "items": {
                "enum": ["full"]
            },
            "minItems": 0,
            "maxItems": 1
        },
        "options": {
            "type": "object",
            "maxProperties": 0
        }
     }
}
)"_json;

static const nlohmann::json init_scheme = R"(
{
     "$schema": "http://json-schema.org/draft-07/schema#",
     "title": "init",
     "type": "object",
     "properties": {
        "name": {
            "type": "string"
        },
        "data": {
            "type": "array",
            "minItems": 1,
            "maxItems": 2,
            "items": {
                "type": "string"
            }
        },
        "options": {
            "type": "object",
            "maxProperties": 3,
            "minProperties": 3,
            "properties": {
                "segm": {
                    "type": "integer",
                    "minimum": 1
                },
                "rec": {
                    "type": "integer",
                    "minimum": 1
                },
                "vol": {
                    "type": "integer",
                    "minimum": 1
                }
            },
            "additionalProperties": false
        }
     }
}
)"_json;

static const nlohmann::json create_scheme = R"(
{
     "$schema": "http://json-schema.org/draft-07/schema#",
     "title": "create",
     "type": "object",
     "properties": {
        "name": {
            "type": "string"
        },
        "data": {
            "type": "array",
            "items": {
                "type": "string"
            },
            "minItems": 1,
            "maxItems": 1
        },
        "options": {
            "type": "object",
            "maxProperties": 1,
            "minProperties": 1,
            "properties": {
                "allocate": {
                    "type": "integer",
                    "minimum": 1
                }
            },
            "additionalProperties": false
        }
     }
}
)"_json;

static const nlohmann::json delete_scheme = R"(
{
     "$schema": "http://json-schema.org/draft-07/schema#",
     "title": "delete",
     "type": "object",
     "properties": {
        "name": {
            "type": "string"
        },
        "data": {
            "type": "array",
            "items": {
                "type": "string"
            },
            "minItems": 1,
            "maxItems": 1
        },
        "options": {
            "type": "object",
            "maxProperties": 0
        }
     }
}
)"_json;

static const nlohmann::json rename_scheme = R"(
{
     "$schema": "http://json-schema.org/draft-07/schema#",
     "title": "rename",
     "type": "object",
     "properties": {
        "name": {
            "type": "string"
        },
        "data": {
            "type": "array",
            "items": {
                "type": "string"
            },
            "minItems": 2,
            "maxItems": 2
        },
        "options": {
            "type": "object",
            "maxProperties": 0
        }
     }
}
)"_json;

static const nlohmann::json copy_scheme = R"(
{
     "$schema": "http://json-schema.org/draft-07/schema#",
     "title": "copy",
     "type": "object",
     "properties": {
        "name": {
            "type": "string"
        },
        "data": {
            "type": "array",
            "items": {
                "type": "string"
            },
            "minItems": 2,
            "maxItems": 2
        },
        "options": {
            "type": "object",
            "maxProperties": 0
        }
     }
}
)"_json;

static const nlohmann::json move_scheme = R"(
{
     "$schema": "http://json-schema.org/draft-07/schema#",
     "title": "move",
     "type": "object",
     "properties": {
        "name": {
            "type": "string"
        },
        "data": {
            "type": "array",
            "items": {
                "type": "string"
            },
            "minItems": 2,
            "maxItems": 2
        },
        "options": {
            "type": "object",
            "maxProperties": 0
        }
     }
}
)"_json;

static const nlohmann::json add_scheme = R"(
{
     "$schema": "http://json-schema.org/draft-07/schema#",
     "title": "add",
     "type": "object",
     "properties": {
        "name": {
            "type": "string"
        },
        "data": {
            "type": "array",
            "items": {
                "type": "string"
            },
            "minItems": 1,
            "maxItems": 1
        },
        "options": {
            "type": "object",
            "maxProperties": 1,
            "minProperties": 1,
            "properties": {
                "size": {
                    "type": "integer",
                    "minimum": 1
                }
            },
            "additionalProperties": false
        }
     }
}
)"_json;

static const nlohmann::json squeeze_scheme = R"(
{
     "$schema": "http://json-schema.org/draft-07/schema#",
     "title": "squeeze",
     "type": "object",
     "properties": {
        "name": {
            "type": "string"
        },
        "data": {
            "type": "array",
            "minItems": 0,
            "maxItems": 0
        },
        "options": {
            "type": "object",
            "maxProperties": 0
        }
     }
}
)"_json;

static const nlohmann::json sort_scheme = R"(
{
     "$schema": "http://json-schema.org/draft-07/schema#",
     "title": "sort",
     "type": "object",
     "properties": {
        "name": {
            "type": "string"
        },
        "data": {
           "type": "array",
            "items": [
                {"enum": ["name", "ext", "date", "size"]},
                {"enum": ["inv"]}
            ],
            "minItems": 0,
            "maxItems": 2
        },
        "options": {
            "type": "object",
            "maxProperties": 0
        }
     }
}
)"_json;

static const nlohmann::json free_scheme = R"(
{
     "$schema": "http://json-schema.org/draft-07/schema#",
     "title": "free",
     "type": "object",
     "properties": {
        "name": {
            "type": "string"
        },
        "data": {
            "type": "array",
            "minItems": 0,
            "maxItems": 0
        },
        "options": {
            "type": "object",
            "maxProperties": 0
        }
     }
}
)"_json;

static const nlohmann::json vol_scheme = R"(
{
     "$schema": "http://json-schema.org/draft-07/schema#",
     "title": "vol",
     "type": "object",
     "properties": {
        "name": {
            "type": "string"
        },
        "data": {
            "type": "array",
            "items": {
                "type": "string"
            },
            "minItems": 1,
            "maxItems": 2
        },
        "options": {
            "type": "object",
            "maxProperties": 0
        }
     }
}
)"_json;

static const nlohmann::json exit_scheme = R"(
{
     "$schema": "http://json-schema.org/draft-07/schema#",
     "title": "exit",
     "type": "object",
     "properties": {
        "name": {
            "type": "string"
        },
        "data": {
            "type": "array",
            "minItems": 0,
            "maxItems": 0
        },
        "options": {
            "type": "object",
            "maxProperties": 0
        }
     }
}
)"_json;

static const nlohmann::json help_scheme = R"(
{
     "$schema": "http://json-schema.org/draft-07/schema#",
     "title": "help",
     "type": "object",
     "properties": {
        "name": {
            "type": "string"
        },
        "data": {
            "type": "array",
            "minItems": 0,
            "maxItems": 1,
            "items": {
                "enum": ["dir", "init", "create", "delete", "rename", "copy", "move", "add", "squeeze", "sort",
                    "free", "vol", "exit", "help"]
            }
        },
        "options": {
            "type": "object",
            "maxProperties": 0
        }
     }
}
)"_json;

static const std::unordered_map<std::string, nlohmann::json> validation_schemes = {
    {"dir", dir_scheme},         {"init", init_scheme}, {"create", create_scheme}, {"delete", delete_scheme},
    {"rename", rename_scheme},   {"copy", copy_scheme}, {"move", move_scheme},     {"add", add_scheme},
    {"squeeze", squeeze_scheme}, {"sort", sort_scheme}, {"free", free_scheme},     {"vol", vol_scheme},
    {"exit", exit_scheme},       {"help", help_scheme}};
