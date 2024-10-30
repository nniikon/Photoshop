#include "ps_plugin_canvas.h"

#include <memory>

#include "ps_canvas.h"
#include "sfm_implementation.h"

bool loadPlugin() {
    std::unique_ptr<psapi::ICanvas> canvas = std::make_unique<ps::Canvas>(
            psapi::vec2i{500, 500},
            psapi::vec2i{300, 300}
    );

    psapi::getRootWindow()->addWindow(std::move(canvas));
    return true;
}
