#pragma once

#include "abstract_command.hpp"

class SortCommand : public AbstractCommand {
   public:
    explicit SortCommand(filesystem::FileSystem& file, const std::string& sort_by, const std::string& inv)
        : AbstractCommand(file), sort_by_(sort_by), inv_(inv){};

    std::string execute() override;

   private:
    std::string sort_by_;
    std::string inv_;
};
