#include "ps_plugin_canvas.h"

#include "loguru.hpp"

#include <memory>

bool loadPlugin() {
    std::unique_ptr<psapi::ICanvas> canvas 
                = std::make_unique<ps::ScrollableDecorator<ps::Canvas>>(
        psapi::vec2i{600, 950},
        psapi::vec2i{300, 50}
    );

    psapi::getRootWindow()->addWindow(std::move(canvas));
    return true;
}
