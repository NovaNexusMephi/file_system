#pragma once

#include "monitor/logging/log_manager.hpp"

namespace monitor {
class Monitor final {
   public:
    Monitor();
    ~Monitor();

   private:
    logging::LogManager log_manager_{};
};
}  // namespace monitor