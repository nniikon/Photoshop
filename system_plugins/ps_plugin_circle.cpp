#include "ps_plugin_circle.h"
#include "ps_plugin_brush.h"

#include "ps_canvas.h"
#include "sfm_implementation.h"

#define LOGURU_WITH_STREAMS 1
#include "loguru.hpp"

#include <cmath>

static psapi::sfm::ITexture* texture = nullptr;

EllipseAction::EllipseAction()
    : ellipse_(psapi::sfm::IEllipseShape::create(1, 1)),
      is_mouse_down_(false) {

    canvas_ = static_cast<psapi::ICanvas*>(
        psapi::getRootWindow()->getWindowById(psapi::kCanvasWindowId)
    );
}

bool EllipseAction::operator()(const psapi::IRenderWindow* renderWindow,
                               const psapi::sfm::Event& event) {
    if (!canvas_)
        return false;

    if (event.type == psapi::sfm::Event::None) {
        is_new_frame_ = true;
    }

    auto active_layer = canvas_->getLayer(canvas_->getActiveLayerIndex());
    auto temp_layer = canvas_->getTempLayer();

    psapi::sfm::vec2i mouse_pos = canvas_->getMousePosition() + canvas_->getPos();

    if (event.type == psapi::sfm::Event::MouseButtonPressed) {
        handleMousePressed(event, mouse_pos);
    }
    else if (event.type == psapi::sfm::Event::MouseButtonReleased) {
        handleMouseReleased(active_layer);
    }
    else if (event.type == psapi::sfm::Event::MouseMoved && is_mouse_down_) {
        if (is_new_frame_) {
            updateTempLayer(temp_layer, mouse_pos);
            is_new_frame_ = false;
        }
    }

    last_mouse_pos_ = mouse_pos;

    return true;
}

bool EllipseAction::activate() {
    return true;
}

void EllipseAction::handleMousePressed(const psapi::sfm::Event& event, const psapi::sfm::vec2i& mouse_pos) {
    if (event.mouseButton.button != psapi::sfm::Mouse::Button::Left) {
        return;
    }

    is_mouse_down_ = true;
    mouse_starting_point_ = mouse_pos;

    ellipse_->setPosition(mouse_starting_point_);
    ellipse_->setFillColor(psapi::sfm::Color{0, 0, 255, 255});  // Set the ellipse color
}

void EllipseAction::handleMouseReleased(psapi::ILayer* active_layer) {
    if (!is_mouse_down_) {
        return;
    }

    is_mouse_down_ = false;
    transferFinalEllipseToLayer(active_layer);
    clearLayer(canvas_->getTempLayer());

    ellipse_->setSize(psapi::sfm::vec2u{1, 1});
    ellipse_->setPosition(psapi::sfm::vec2i{0, 0});
}

void EllipseAction::updateTempLayer(psapi::ILayer* layer, const psapi::sfm::vec2i& current_pos) {
    psapi::sfm::vec2i size = {
        std::abs(current_pos.x - mouse_starting_point_.x),
        std::abs(current_pos.y - mouse_starting_point_.y)
    };

    if (size.x == 0 || size.y == 0) {
        return;
    }

    ellipse_->setSize(psapi::sfm::vec2u{static_cast<unsigned int>(size.x),
                                        static_cast<unsigned int>(size.y)});
    ellipse_->setPosition(psapi::sfm::vec2i{
        std::min(mouse_starting_point_.x, current_pos.x),
        std::min(mouse_starting_point_.y, current_pos.y)
    });

    LOG_F(INFO, "Ellipse size: %d, %d", size.x, size.y);
    LOG_F(INFO, "Ellipse position: %d, %d", ellipse_->getPosition().x, ellipse_->getPosition().y);

    clearLayer(layer);
    transferFinalEllipseToLayer(layer);
}

void EllipseAction::clearLayer(psapi::ILayer* layer) {
    size_t width  = canvas_->getSize().x;
    size_t height = canvas_->getSize().y;

    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            layer->setPixel({static_cast<int>(x),
                             static_cast<int>(y)},
                             psapi::sfm::Color{0, 0, 0, 0});
        }
    }
}

void EllipseAction::transferFinalEllipseToLayer(psapi::ILayer* active_layer) {
    auto temp_image = ellipse_->getImage();
    auto text = psapi::sfm::ITexture::create();

    unsigned int x_offset = static_cast<unsigned int>(canvas_->getPos().x);
    unsigned int y_offset = static_cast<unsigned int>(canvas_->getPos().y);

    for (unsigned y = 0; y < canvas_->getSize().y; ++y) {
        for (unsigned x = 0; x < canvas_->getSize().x; ++x) {
            psapi::sfm::Color pixel_color = temp_image->getPixel(x + x_offset,
                                                                 y + y_offset);
            if (pixel_color.a != 0) {
                active_layer->setPixel(psapi::sfm::vec2i{static_cast<int>(x),
                                                         static_cast<int>(y)}, pixel_color);
            }
        }
    }
}

constexpr psapi::sfm::IntRect kEllipseButtonTextureArea = {128, 64, 64, 64};

bool loadPlugin() {
    texture = psapi::sfm::ITexture::create().release();
    texture->loadFromFile("./assets/buttons.png");

    auto toolbar_sprite = psapi::sfm::ISprite::create();
    toolbar_sprite->setTexture(texture);
    toolbar_sprite->setTextureRect(kEllipseButtonTextureArea);

    auto toolbar = dynamic_cast<psapi::IBar*>(psapi::getRootWindow()->getWindowById(psapi::kToolBarWindowId));

    auto ellipse_action = std::make_unique<EllipseAction>();
    auto ellipse_button = std::make_unique<ps::ABarButton>(std::move(toolbar_sprite),
                                                           toolbar,
                                                           std::move(ellipse_action));
    toolbar->addWindow(std::move(ellipse_button));

    return true;
}

void unloadPlugin() {
    delete texture;
}
