#pragma once

#include <string>
namespace monitor {
class Writer final {
   public:
    Writer() = delete;

    void write(int status, std::string message);
};
}  // namespace monitor