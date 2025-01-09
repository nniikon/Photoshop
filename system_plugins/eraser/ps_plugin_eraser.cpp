#include "ps_plugin_eraser.h"
#include "loguru.hpp"

#include "ps_parse_sprite.h"

static psapi::sfm::ITexture* texture = nullptr;

ClearAction::ClearAction() {
    canvas_ = static_cast<psapi::ICanvas*>(psapi::getRootWindow()->getWindowById(psapi::kCanvasWindowId));
}

void ClearAction::clearCanvas() {
    if (!canvas_)
        return;

    auto layer = canvas_->getLayer(canvas_->getActiveLayerIndex());
    auto canvas_size = canvas_->getSize();

    for (int x = 0; x < canvas_size.x; x++) {
        for (int y = 0; y < canvas_size.y; y++) {
            layer->setPixel({x, y}, psapi::sfm::Color{255, 255, 255, 255});
        }
    }
}

bool ClearAction::activate() {
    clearCanvas();
    return false;
}

bool ClearAction::operator()(const psapi::IRenderWindow* renderWindow,
                             const psapi::sfm::Event& event) {
    return true;
}

bool onLoadPlugin() {
    ps::SpriteInfo sprite_info = ps::ParseSpriteFromConfig("system_plugins/eraser/ps_plugin_eraser_config.pscfg");
    texture = sprite_info.texture.release();

    auto toolbar = dynamic_cast<psapi::IBar*>(psapi::getRootWindow()->getWindowById(psapi::kToolBarWindowId));

    auto clear_action = std::make_unique<ClearAction>();
    auto clear_button = std::make_unique<ps::ABarButton>(std::move(sprite_info.sprite),
                                                         toolbar,
                                                         std::move(clear_action));
    toolbar->addWindow(std::move(clear_button));

    return true;
}

void onUnloadPlugin() {
    delete texture;
}
