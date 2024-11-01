#include "ps_plugin_rectangle.h"
#include "ps_plugin_brush.h"

#include "ps_canvas.h"
#include "sfm_implementation.h"

#include <cmath>

static psapi::sfm::ITexture* texture = nullptr;

RectangleAction::RectangleAction()
    : rect_(psapi::sfm::IRectangleShape::create(1, 1)),
      is_mouse_down_(false) {

    canvas_ = static_cast<psapi::ICanvas*>(
        psapi::getRootWindow()->getWindowById(psapi::kCanvasWindowId)
    );
}

bool RectangleAction::operator()(const psapi::IRenderWindow* renderWindow,
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

bool RectangleAction::activate() {
    return true;
}

void RectangleAction::handleMousePressed(const psapi::sfm::Event& event, const psapi::sfm::vec2i& mouse_pos) {
    if (event.mouseButton.button != psapi::sfm::Mouse::Button::Left) {
        return;
    }

    is_mouse_down_ = true;
    mouse_starting_point_ = mouse_pos;

    rect_->setPosition(mouse_starting_point_);
    rect_->setFillColor(psapi::sfm::Color{255, 0, 0, 255});  // Set the rectangle color
}

void RectangleAction::handleMouseReleased(psapi::ILayer* active_layer) {
    if (!is_mouse_down_) {
        return;
    }

    is_mouse_down_ = false;
    transferFinalRectangleToLayer(active_layer);
    clearLayer(canvas_->getTempLayer());

    rect_->setSize(psapi::sfm::vec2u{1, 1});
    rect_->setPosition(psapi::sfm::vec2i{0, 0});
}

void RectangleAction::updateTempLayer(psapi::ILayer* layer, const psapi::sfm::vec2i& current_pos) {
    psapi::sfm::vec2i size = {
        std::abs(current_pos.x - mouse_starting_point_.x),
        std::abs(current_pos.y - mouse_starting_point_.y)
    };

    rect_->setSize(psapi::sfm::vec2u{size.x, size.y});
    rect_->setPosition(psapi::sfm::vec2i{
        std::min(mouse_starting_point_.x, current_pos.x),
        std::min(mouse_starting_point_.y, current_pos.y)
    });

    clearLayer(layer);
    transferFinalRectangleToLayer(layer);
}

void RectangleAction::clearLayer(psapi::ILayer* layer) {
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

void RectangleAction::transferFinalRectangleToLayer(psapi::ILayer* active_layer) {
    auto temp_image = rect_->getImage();
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

constexpr psapi::sfm::IntRect kRectangleButtonTextureArea = {64, 64, 64, 64};

bool loadPlugin() {
    texture = psapi::sfm::ITexture::create().release();
    texture->loadFromFile("./assets/buttons.png");

    auto toolbar_sprite = psapi::sfm::ISprite::create();
    toolbar_sprite->setTexture(texture);
    toolbar_sprite->setTextureRect(kRectangleButtonTextureArea);

    auto toolbar = dynamic_cast<psapi::IBar*>(psapi::getRootWindow()->getWindowById(psapi::kToolBarWindowId));

    auto rectangle_action = std::make_unique<RectangleAction>();
    auto rectangle_button = std::make_unique<ps::ABarButton>(std::move(toolbar_sprite),
                                                             toolbar,
                                                             std::move(rectangle_action));
    toolbar->addWindow(std::move(rectangle_button));

    return true;
}

void unloadPlugin() {
    delete texture;
}
