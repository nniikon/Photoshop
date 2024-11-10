#include "ps_plugin_toolbar.h"

#include "ps_bar.h"
#include "sfm_implementation.h"

namespace ps {

using psapi::sfm::vec2i;
using psapi::sfm::vec2f;

} // namespace

static psapi::sfm::ITexture* texture = nullptr;

constexpr psapi::sfm::IntRect kToolbarTextureArea       = {0,   0,   256, 1024};
constexpr psapi::sfm::IntRect kHoverButtonTextureArea   = {256, 0,   64,  64};
constexpr psapi::sfm::IntRect kPressedButtonTextureArea = {256, 64,  64,  64};
constexpr psapi::sfm::IntRect kActiveButtonTextureArea  = {256, 128, 64,  64};
    
bool loadPlugin() {

    texture = psapi::sfm::ITexture::create().release();
    texture->loadFromFile("./assets/toolbar.png");

    std::unique_ptr<psapi::sfm::ISprite> toolbar_sprite = psapi::sfm::Sprite::create();
    toolbar_sprite->setTexture(texture);
    toolbar_sprite->setTextureRect(kToolbarTextureArea);

    std::unique_ptr<psapi::sfm::ISprite> hover_button_sprite = psapi::sfm::Sprite::create();
    hover_button_sprite->setTexture(texture);
    hover_button_sprite->setTextureRect(kHoverButtonTextureArea);

    std::unique_ptr<psapi::sfm::ISprite> pressed_button_sprite = psapi::sfm::Sprite::create();
    pressed_button_sprite->setTexture(texture);
    pressed_button_sprite->setTextureRect(kPressedButtonTextureArea);

    std::unique_ptr<psapi::sfm::ISprite> active_button_sprite = psapi::sfm::Sprite::create();
    active_button_sprite->setTexture(texture);
    active_button_sprite->setTextureRect(kActiveButtonTextureArea);

    auto toolbar = std::make_unique<ps::ABar>(std::move(toolbar_sprite),
                                              std::move(hover_button_sprite),
                                              std::move(pressed_button_sprite),
                                              std::move(active_button_sprite),
                                              psapi::vec2i{0, 0},
                                              psapi::vec2i{256, 1024},
                                              psapi::vec2i{32, 32},
                                              psapi::vec2i{64, 64},
                                              psapi::vec2i{64, 64},
                                              2,
                                              psapi::kToolBarWindowId);

    psapi::getRootWindow()->addWindow(std::move(toolbar));

    return true;
}

void unloadPlugin() {
    delete texture;
}
