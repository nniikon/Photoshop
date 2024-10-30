#include "ps_plugin_line.h"

#define LOGURU_WITH_STREAMS 1
#include "loguru.hpp"

#include "api_canvas.hpp"
#include "ps_bar.h"
#include "sfm_implementation.h"

static psapi::sfm::ITexture* texture = nullptr;

class LineAction : public ps::ButtonAction {
public:
    LineAction();
    virtual bool operator()(const psapi::IRenderWindow* renderWindow, 
                            const psapi::sfm::Event& event) override;

private:
    psapi::ICanvas* canvas_ = nullptr;
};

LineAction::LineAction() {
    canvas_ = static_cast<psapi::ICanvas*>
        (psapi::getRootWindow()->getWindowById(psapi::kCanvasWindowId));
}

bool LineAction::operator()(const psapi::IRenderWindow* renderWindow,
                             const psapi::sfm::Event& event) {
    if (!canvas_) {
        return false;
    }

    if (!canvas_->isPressed()) {
        return false;
    }

    LOG_F(INFO, "Mouse at (%d, %d)", canvas_->getMousePosition().x,
                                     canvas_->getMousePosition().y);

    auto layer = canvas_->getLayer(canvas_->getActiveLayerIndex());

    // Paint 5x5 area around the mouse
    for (int x = -2; x < 3; x++) {
        for (int y = -2; y < 3; y++) {
            psapi::sfm::vec2i mouse_pos_ = {canvas_->getMousePosition().x + x, canvas_->getMousePosition().y + y};
            if (mouse_pos_.x < 0 || mouse_pos_.x >= canvas_->getSize().x ||
                mouse_pos_.y < 0 || mouse_pos_.y >= canvas_->getSize().y) {
                continue;
            }

            layer->setPixel({canvas_->getMousePosition().x + x, canvas_->getMousePosition().y + y},
                            psapi::sfm::Color{255, 0, 0, 255});
        }
    }

    return true;
}

constexpr psapi::sfm::IntRect kBrushButtonTextureArea = {0, 64, 64, 64};

bool loadPlugin() {

    texture = psapi::sfm::Texture::create().release();
    texture->loadFromFile("./assets/buttons.png");

    std::unique_ptr<psapi::sfm::ISprite> toolbar_sprite = psapi::sfm::Sprite::create();
    toolbar_sprite->setTexture(texture);
    toolbar_sprite->setTextureRect(kBrushButtonTextureArea);

    psapi::IBar* toolbar = 
        dynamic_cast<psapi::IBar*>(psapi::getRootWindow()->getWindowById(psapi::kToolBarWindowId));

    auto line_action = std::make_unique<LineAction>();

    auto line_button = std::make_unique<ps::ABarButton>(
        std::move(toolbar_sprite),
        toolbar,
        std::move(line_action)
    );

    toolbar->addWindow(std::move(line_button));

    return true;
}

void unloadPlugin() {
    delete texture;
}


