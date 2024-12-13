#include "ps_plugin_secondary_panel.h"

#include "ps_parse_sprite.h"

#include "loguru.hpp"

#include <cassert>

bool loadPlugin() {
    auto secondary_panel = std::make_unique<ps::SecondaryPanel>();

    psapi::getRootWindow()->addWindow(std::move(secondary_panel));
    return true;
}

void unloadPlugin() {
}

using namespace ps;

SecondaryPanel::SecondaryPanel() 
    : color_palette_(kSecondaryPanelColorPalette, this) {
    const char config_path[] = "system_plugins/secondary_panel/ps_plugin_secondary_panel_config.pscfg";
    auto sprite_info = ParseSpriteFromConfig(config_path);

    sprite_  = std::move(sprite_info.sprite);
    texture_ = std::move(sprite_info.texture);
    assert(sprite_ && texture_);
}

void SecondaryPanel::draw(psapi::sfm::IRenderWindow* render_window) {
    unsigned int pos_x = psapi::getRootWindow()->getSize().x - sprite_->getSize().x;

    LOG_F(INFO, "RootWindow pos: %d x %d", psapi::getRootWindow()->getSize().x, 0);
    LOG_F(INFO, "Sprite pos: %d x %d", pos_x, 0);
    sprite_->setPosition(static_cast<float>(pos_x), 0.0f);
    LOG_F(INFO, "Sprite pos: %f x %f", sprite_->getPosition().x, sprite_->getPosition().y);
    LOG_F(INFO, "Sprite size: %d x %d", sprite_->getSize().x, sprite_->getSize().y);
    sprite_->draw(render_window);

    color_palette_.draw(render_window);
}

class SecondaryPanelAction : public psapi::IAction {
public:
    SecondaryPanelAction(const psapi::IRenderWindow* render_window,
                         const psapi::Event& event,
                         const psapi::sfm::ISprite* sprite,
                         ColorPalette* color_palette)
        : render_window_(render_window),
          event_(event),
          sprite_(sprite),
          color_palette_(color_palette) {
    }

    bool execute(const Key& key) {
        psapi::vec2i mouse_pos = psapi::sfm::Mouse::getPosition(render_window_);

        psapi::vec2i pos =  {static_cast<int>(sprite_->getPosition().x),
                             static_cast<int>(sprite_->getPosition().y)};
        psapi::vec2i size = {static_cast<int>(sprite_->getSize().x),
                             static_cast<int>(sprite_->getSize().y)};

        bool is_in_window = pos.x <= mouse_pos.x && mouse_pos.x < pos.x + size.x &&
                            pos.y <= mouse_pos.y && mouse_pos.y < pos.y + size.y;

        if (!is_in_window) {
            return false;
        }

        auto action_controller = psapi::getActionController();

        return action_controller->execute(color_palette_->createAction(render_window_, event_));
    }

    bool isUndoable(const Key& key) {
        return false;
    }

private:
    const psapi::IRenderWindow* render_window_ = nullptr;
    const psapi::Event& event_;
    const psapi::sfm::ISprite* sprite_ = nullptr;
    ColorPalette* color_palette_ = nullptr;
};

std::unique_ptr<psapi::IAction> SecondaryPanel::createAction(const psapi::IRenderWindow* renderWindow,
                                                             const psapi::Event& event) {
    return std::make_unique<SecondaryPanelAction>(renderWindow, event, sprite_.get(), &color_palette_);
}

void SecondaryPanel::addWindow(std::unique_ptr<psapi::IWindow> window) {
    windows_.push_back(std::move(window));
}

void SecondaryPanel::removeWindow(psapi::wid_t id) {
    for (auto it = windows_.begin(); it != windows_.end(); it++) {
        if ((*it)->getId() == id) {
            windows_.erase(it);
            return;
        }
    }
}

void SecondaryPanel::setPos(const psapi::vec2i& pos) { 
    assert(0);
}

void SecondaryPanel::setSize(const psapi::vec2u& size) { 
    assert(0);
}

psapi::wid_t SecondaryPanel::getId() const {
    return kSecondaryPanelWindowId;
}

psapi::IWindow* SecondaryPanel::getWindowById(psapi::wid_t id) {
    return const_cast<IWindow*>(static_cast<const SecondaryPanel*>(this)->getWindowById(id));
}

class WindowContainerAction : public psapi::IAction
{
public:
    WindowContainerAction(std::vector<std::unique_ptr<psapi::IWindow>>& windows_,
                          const psapi::IRenderWindow* render_window,
                          const psapi::sfm::Event& event)
        : windows_     (windows_),
          render_window_(render_window),
          event(event) {}

    bool execute(const psapi::IAction::Key& key) override {
        auto action_controller = psapi::getActionController();
        bool result = false;
        for (auto& window : windows_) {
            result |= action_controller->execute(window->createAction(render_window_, event));
        }
        return result;
    }

    bool isUndoable(const psapi::IAction::Key&) override {
        return false;
    }

private:
    std::vector<std::unique_ptr<psapi::IWindow>>& windows_;
    const psapi::IRenderWindow* render_window_;
    const psapi::sfm::Event& event;
};

const psapi::IWindow* SecondaryPanel::getWindowById(psapi::wid_t id) const {
    if (id == kSecondaryPanelWindowId) {
        return this;
    }

    for (const auto& window : windows_) {
        IWindow* result = window->getWindowById(id);
        if (result) {
            return result;
        }
    }
    
    return nullptr;
}

psapi::vec2i SecondaryPanel::getPos() const {
    return {static_cast<int>(sprite_->getPosition().x),
            static_cast<int>(sprite_->getPosition().y)};
}

psapi::vec2u SecondaryPanel::getSize() const {
    return {static_cast<unsigned int>(sprite_->getSize().x),
            static_cast<unsigned int>(sprite_->getSize().y)};
}

void SecondaryPanel::setParent(const IWindow* parent) {
    parent_ = parent;
}

void SecondaryPanel::forceActivate() {
    assert(0);
}

void SecondaryPanel::forceDeactivate() {
    assert(0);
}

bool SecondaryPanel::isActive() const {
    assert(0);
}

void SecondaryPanel::enable() {
    is_enabled_ = true;
}

void SecondaryPanel::disable() {
    is_enabled_ = false;
}

Color SecondaryPanel::getColor() const {
    return color_palette_.getColor();
}

void SecondaryPanel::setColor(Color color) {
    return color_palette_.setColor(color);
}

int SecondaryPanel::getThickness() const {
    (void)getWindowById(kSecondaryPanelThicknessSlider);

    return 1;
}

void SecondaryPanel::setThickness(int thickness) {
    (void)getWindowById(kSecondaryPanelThicknessSlider);
}

SecondaryPanelButton::SecondaryPanelButton(psapi::wid_t id,
                                           const SecondaryPanel* parent)
    : parent_panel_(static_cast<const SecondaryPanel*>(parent)),
      id_(id) {
}

psapi::wid_t SecondaryPanelButton::getId() const {
    return id_;
}

const psapi::IWindow* SecondaryPanelButton::getWindowById(psapi::wid_t id) const {
    if (id == id_) {
        return this;
    }
    
    return nullptr;
}

psapi::IWindow* SecondaryPanelButton::getWindowById(psapi::wid_t id) {
    return const_cast<IWindow*>(static_cast<const SecondaryPanelButton*>(this)->getWindowById(id));
}

psapi::vec2i SecondaryPanelButton::getPos() const {
    return {0, 0};
}

void SecondaryPanelButton::setPos(const psapi::vec2i& pos) {
    assert(0);
}

void SecondaryPanelButton::setSize(const psapi::vec2u& size) {
    assert(0);
}

psapi::vec2u SecondaryPanelButton::getSize() const {
    return {0, 0};
}

void SecondaryPanelButton::setParent(const psapi::IWindow* parent) {
    parent_panel_ = static_cast<const SecondaryPanel*>(parent);
}

void SecondaryPanelButton::forceActivate() {
    assert(0);
}

void SecondaryPanelButton::forceDeactivate() {
    assert(0);
}

bool SecondaryPanelButton::isActive() const {
    assert(0);
    return false;
}

bool SecondaryPanelButton::isWindowContainer() const {
    return false;
}

const size_t kColorPaletteWidthColors  = 6;
const size_t kColorPaletteHeightColors = 2;
const size_t kColorSize                = 32;

const Color kColorPaletteColors[kColorPaletteHeightColors][kColorPaletteWidthColors] = {
    {
        {0  , 0  , 0  , 255},
        {0  , 0  , 255, 255},
        {0  , 255, 0  , 255},
        {0  , 255, 255, 255},
        {255, 0  , 0  , 255},
        {255, 0  , 255, 255},
    },
    {
        {128, 0  , 0  , 255},
        {128, 0  , 128, 255},
        {128, 128, 0  , 255},
        {128, 128, 128, 255},
        {0  , 128, 0  , 255},
        {0  , 128, 128, 255},
    }
};

ColorPalette::ColorPalette(psapi::wid_t id, const SecondaryPanel* parent_panel)
    : SecondaryPanelButton(id, parent_panel) {
}

void ColorPalette::draw(psapi::IRenderWindow* renderWindow) {
    using psapi::sfm::IRectangleShape;

    std::unique_ptr<IRectangleShape> rect = IRectangleShape::create(kColorSize, kColorSize);

    size_t panel_x = static_cast<size_t>(parent_panel_->getPos().x);
    size_t panel_y = static_cast<size_t>(parent_panel_->getPos().y);

    for (size_t y = 0; y < kColorPaletteHeightColors; y += 1) {
        for (size_t x = 0; x < kColorPaletteWidthColors; x += 1) {
            rect->setPosition(
                psapi::vec2i{ static_cast<int>(panel_x + x * kColorSize + kColorSize),
                              static_cast<int>(panel_y + y * kColorSize + kColorSize) });
            rect->setFillColor(kColorPaletteColors[y][x]);
            rect->draw(renderWindow);
        }
    }

    rect->setPosition(psapi::vec2i{static_cast<int>(panel_x + kColorSize),
                                   static_cast<int>(panel_y + kColorSize)});
    rect->setSize(psapi::vec2u{static_cast<unsigned int>(kColorSize * kColorPaletteWidthColors),
                               static_cast<unsigned int>(kColorSize * kColorPaletteHeightColors)});
    rect->setFillColor({0, 0, 0, 0});
    rect->setOutlineColor({255, 255, 255, 255});
    rect->setOutlineThickness(2);
    rect->draw(renderWindow);
}

class ColorPaletteAction : public psapi::IAction {
public:
    ColorPaletteAction(const psapi::IRenderWindow* render_window_,
                       const psapi::Event& event,
                       const SecondaryPanel* parent_panel_,
                       Color* color)
        : render_window_(render_window_),
          event_(event),
          parent_panel_(parent_panel_),
          color_(color) {
    }

    bool execute(const Key&) override {
        psapi::vec2i mouse_pos = psapi::sfm::Mouse::getPosition(render_window_);

        psapi::vec2i pos =  {static_cast<int>(parent_panel_->getPos().x + (int)kColorSize),
                             static_cast<int>(parent_panel_->getPos().y + (int)kColorSize)};
        psapi::vec2i size = {static_cast<int>(kColorPaletteWidthColors  * kColorSize),
                             static_cast<int>(kColorPaletteHeightColors * kColorSize)};

        bool is_in_window = pos.x <= mouse_pos.x && mouse_pos.x < pos.x + size.x &&
                            pos.y <= mouse_pos.y && mouse_pos.y < pos.y + size.y;

        if (!is_in_window || event_.type != psapi::Event::MouseButtonReleased) {
            return false;
        }

        int color_index_x = (mouse_pos.x - pos.x) / (int)kColorSize;
        int color_index_y = (mouse_pos.y - pos.y) / (int)kColorSize;

        *color_ = kColorPaletteColors[color_index_y][color_index_x];

        LOG_F(INFO, "Color selected: %d, %d", color_index_x, color_index_y);
        LOG_F(INFO, "Color: %d, %d, %d, %d", color_->r, color_->g, color_->b, color_->a);

        return false;
    }

    bool isUndoable(const Key&) override {
        return false; 
    }

private:
    const psapi::IRenderWindow* render_window_;
    const psapi::Event& event_;
    const SecondaryPanel* parent_panel_;

    Color* color_;
};

std::unique_ptr<psapi::IAction> ColorPalette::createAction(const psapi::IRenderWindow* renderWindow,
                                             const psapi::Event& event) {
    return std::make_unique<ColorPaletteAction>(renderWindow, event, parent_panel_, &color_);
}
Color ColorPalette::getColor() const {
    return color_;
}

void ColorPalette::setColor(Color color) {
    color_ = color;
}
