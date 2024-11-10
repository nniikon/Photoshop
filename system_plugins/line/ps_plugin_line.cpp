#include "ps_plugin_line.h"

#include "ps_parse_sprite.h"

#include <string>
#include <immintrin.h>

static psapi::sfm::ITexture* texture = nullptr;

LineAction::LineAction()
    : rect_(psapi::sfm::IRectangleShape::create(1, 1)),
      is_mouse_down_(false) {

    canvas_ = static_cast<psapi::ICanvas*>(
        psapi::getRootWindow()->getWindowById(psapi::kCanvasWindowId)
    );
}

bool LineAction::operator()(const psapi::IRenderWindow* renderWindow,
                            const psapi::sfm::Event& event) {
    if (!canvas_)
        return false;

    if (event.type == psapi::sfm::Event::None) {
        is_new_frame_ = true;
    }

    auto active_layer = canvas_->getLayer(canvas_->getActiveLayerIndex());
    auto   temp_layer = canvas_->getTempLayer();

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

bool LineAction::activate() {
    return true;
}

void LineAction::handleMousePressed(const psapi::sfm::Event& event, const psapi::sfm::vec2i& mouse_pos) {
    if (event.mouseButton.button != psapi::sfm::Mouse::Button::Left) {
        return;
    }

    is_mouse_down_ = true;
    mouse_starting_point_ = mouse_pos;
    initializeRectangle();
}

void LineAction::initializeRectangle() {
    rect_->setPosition(mouse_starting_point_);
    rect_->setFillColor(psapi::sfm::Color{255, 255, 0, 255});
}

void LineAction::handleMouseReleased(psapi::ILayer* active_layer) {
    if (!is_mouse_down_) {
        return;
    }

    is_mouse_down_ = false;
    transferFinalLineToLayer(active_layer);
    clearLayer(canvas_->getTempLayer());

    rect_->setSize(psapi::sfm::vec2u{1, 1});
    rect_->setPosition(psapi::sfm::vec2i{0, 0});
}

void LineAction::updateTempLayer(psapi::ILayer* layer, const psapi::sfm::vec2i& current_pos) {
    float length = 0.f,
          angle  = 0.f;
    std::tie(length, angle) = calculateLineVector(current_pos);
    setupRectangle(length, angle);
    clearLayer(layer);
    
    transferFinalLineToLayer(layer);
}

void LineAction::clearLayer(psapi::ILayer* layer) {
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

std::pair<float, float> LineAction::calculateLineVector(const psapi::sfm::vec2i& current_pos) {
    psapi::sfm::vec2f line_vector = {
        static_cast<float>(current_pos.x - mouse_starting_point_.x),
        static_cast<float>(current_pos.y - mouse_starting_point_.y)
    };

    float length = std::sqrt(line_vector.x * line_vector.x + line_vector.y * line_vector.y);
    float  angle = std::atan2(line_vector.y, line_vector.x) * 180.0f / M_PIf;
    return {length, angle};
}

void LineAction::setupRectangle(float length, float angle) {
    if (length < 0.1f)
        return;

    rect_->setSize({static_cast<unsigned int>(length), 5});  // Width = length,
                                                             // height = thickness
    rect_->setRotation(angle);
    rect_->setPosition(mouse_starting_point_);
}

void LineAction::transferFinalLineToLayer(psapi::ILayer* active_layer) {

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

bool loadPlugin() {
    ps::SpriteInfo sprite_info = ps::ParseSpriteFromConfig("system_plugins/line/ps_plugin_line_config.pscfg");
    texture = sprite_info.texture.release();

    auto toolbar = dynamic_cast<psapi::IBar*>(psapi::getRootWindow()->getWindowById(psapi::kToolBarWindowId));

    auto line_action = std::make_unique<LineAction>();
    auto line_button = std::make_unique<ps::ABarButton>(std::move(sprite_info.sprite),
                                                        toolbar,
                                                        std::move(line_action));
    toolbar->addWindow(std::move(line_button));

    return true;
}

void unloadPlugin() {
    delete texture;
}
