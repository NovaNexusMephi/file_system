#pragma once

#include <ostream>

namespace monitor {
class Writer final {
   public:
    Writer() = default;
    void write(std::ostream& to, const std::string& message);
};
}  // namespace monitor