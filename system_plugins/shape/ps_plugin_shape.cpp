#include "ps_plugin_shape.h"

#include "ps_parse_sprite.h"
#include "ps_plugin_secondary_panel.h"

#include <cmath>

static psapi::sfm::ITexture*    rect_texture = nullptr;
static psapi::sfm::ITexture* ellipse_texture = nullptr;

template <typename T>
ShapeAction<T>::ShapeAction()
    : ellipse_(T::create(1, 1)),
      is_mouse_down_(false) {

    canvas_ = static_cast<psapi::ICanvas*>(
        psapi::getRootWindow()->getWindowById(psapi::kCanvasWindowId)
    );
}

template <typename T>
bool ShapeAction<T>::operator()(const psapi::IRenderWindow* renderWindow,
                                const psapi::sfm::Event& event) {
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

template <typename T>
bool ShapeAction<T>::activate() {
    return true;
}

template <typename T>
void ShapeAction<T>::handleMousePressed(const psapi::sfm::Event& event, const psapi::sfm::vec2i& mouse_pos) {
    if (event.mouseButton.button != psapi::sfm::Mouse::Button::Left) {
        return;
    }

    is_mouse_down_ = true;
    mouse_starting_point_ = mouse_pos;

    ellipse_->setPosition(mouse_starting_point_);

    auto secondary_panel = dynamic_cast<const ps::SecondaryPanel*>(psapi::getRootWindow()->getWindowById(ps::kSecondaryPanelWindowId));
    ellipse_->setFillColor(secondary_panel->getColor());
}

template <typename T>
void ShapeAction<T>::handleMouseReleased(psapi::ILayer* active_layer) {
    if (!is_mouse_down_) {
        return;
    }

    is_mouse_down_ = false;
    transferFinalShapeToLayer(active_layer);
    clearLayer(canvas_->getTempLayer());

    ellipse_->setSize(psapi::sfm::vec2u{1, 1});
    ellipse_->setPosition(psapi::sfm::vec2i{0, 0});
}

template <typename T>
void ShapeAction<T>::updateTempLayer(psapi::ILayer* layer, const psapi::sfm::vec2i& current_pos) {
    psapi::sfm::vec2i size = {
        std::abs(current_pos.x - mouse_starting_point_.x),
        std::abs(current_pos.y - mouse_starting_point_.y)
    };

    ellipse_->setSize(psapi::sfm::vec2u{static_cast<unsigned int>(size.x),
                                        static_cast<unsigned int>(size.y)});
    ellipse_->setPosition(psapi::sfm::vec2i{
        std::min(mouse_starting_point_.x, current_pos.x),
        std::min(mouse_starting_point_.y, current_pos.y)
    });

    clearLayer(layer);
    transferFinalShapeToLayer(layer);
}

template <typename T>
void ShapeAction<T>::clearLayer(psapi::ILayer* layer) {
    size_t width  = canvas_->getSize().x;
    size_t height = canvas_->getSize().y;

    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            layer->setPixel({static_cast<int>(x),
                             static_cast<int>(y)},
                             psapi::sfm::Color{0, 0, 0, 0});
        }
    }
}

template <typename T>
void ShapeAction<T>::transferFinalShapeToLayer(psapi::ILayer* active_layer) {
    auto temp_image = ellipse_->getImage();
    auto text = psapi::sfm::ITexture::create();

    unsigned int x_offset = static_cast<unsigned int>(canvas_->getPos().x);
    unsigned int y_offset = static_cast<unsigned int>(canvas_->getPos().y);

    for (unsigned y = 0; y < canvas_->getSize().y; y++) {
        for (unsigned x = 1; x < canvas_->getSize().x; x++) {
            psapi::sfm::Color pixel_color = temp_image->getPixel(x + x_offset,
                                                                 y + y_offset);
            if (pixel_color.a != 0) {
                active_layer->setPixel(psapi::sfm::vec2i{static_cast<int>(x),
                                                         static_cast<int>(y)}, pixel_color);
            }
        }
    }
}

bool onLoadPlugin() {
    ps::SpriteInfo    rect_sprite_info = ps::ParseSpriteFromConfig("system_plugins/shape/ps_plugin_rect_config.pscfg");
    ps::SpriteInfo ellipse_sprite_info = ps::ParseSpriteFromConfig("system_plugins/shape/ps_plugin_ellipse_config.pscfg");

    rect_texture    = rect_sprite_info.texture.release();
    ellipse_texture = ellipse_sprite_info.texture.release();

    auto toolbar = dynamic_cast<psapi::IBar*>(psapi::getRootWindow()->getWindowById(psapi::kToolBarWindowId));

    auto ellipse_action = std::make_unique<ShapeAction<psapi::sfm::IEllipseShape>>();
    auto ellipse_button = std::make_unique<ps::ABarButton>(std::move(ellipse_sprite_info.sprite),
                                                           toolbar,
                                                           std::move(ellipse_action));
    toolbar->addWindow(std::move(ellipse_button));

    auto rect_action = std::make_unique<ShapeAction<psapi::sfm::IRectangleShape>>();
    auto rect_button = std::make_unique<ps::ABarButton>(std::move(rect_sprite_info.sprite),
                                                        toolbar,
                                                        std::move(rect_action));
    toolbar->addWindow(std::move(rect_button));

    return true;
}

void onUnloadPlugin() {
    delete    rect_texture;
    delete ellipse_texture;
}
