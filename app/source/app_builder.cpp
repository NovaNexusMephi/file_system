#include "app/app_builder.h"

#include <filesystem>
#include <memory>

#include "app/app.h"

using app::App;
using app::AppBuilder;

AppBuilder& AppBuilder::set_cfg_path(const std::filesystem::path& cfg_path) {
    cfg_path_ = cfg_path;
    return *this;
}

std::unique_ptr<App> AppBuilder::build() {
    auto app = std::make_unique<App>(cfg_path_);
    return app;
}