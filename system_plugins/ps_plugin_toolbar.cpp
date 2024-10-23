#include "ps_plugin_toolbar.h"

#include <cassert>
#include <iostream>
#include <memory>

extern "C" {
    
bool loadPlugin() {
    psapi::vec2i pos  = {0,   0  };
    psapi::vec2i size = {128, 128};
    auto toolbar = std::make_unique<ps::Toolbar>(pos, size);

    psapi::getRootWindow()->addWindow(std::move(toolbar));

    return true;
}

void unloadPlugin() {
}

}

using namespace ps;

Toolbar::Toolbar(vec2i pos, vec2i size) {}

void Toolbar::draw(psapi::ARenderWindow* renderWindow) {
    renderWindow->draw(&sprite_);

    std::cerr << "Toolbar::draw" << std::endl;

    drawChildren(renderWindow);
}

bool Toolbar::update(const psapi::ARenderWindow* renderWindow,
                     const psapi::sfm::Event& event) {
    return updateChildren(renderWindow, event);
}

void Toolbar::addChild(std::unique_ptr<ToolbarButton> button) {
    addWindow(std::move(button));
    num_children_++;
}

vec2i Toolbar::getNextChildPos()  const {
    vec2i pos = {gap_size, gap_size};
    pos.x += child_size_.x * next_child_index_;
    
    return pos;
}

vec2i Toolbar::getNextChildSize() const {
    return child_size_;
}

void Toolbar::finishButtonDraw(psapi::ARenderWindow* renderWindow, const psapi::IBarButton* button) {
}
