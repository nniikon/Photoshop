#include "ps_canvas.h"

#define LOGURU_WITH_STREAMS 1
#include "loguru.hpp"

#include <cassert>

using namespace ps;

Layer::Layer(vec2i size)
    : size_(size),
      pixels_(static_cast<size_t>(size.x) *
              static_cast<size_t>(size.y))
{}

Color Layer::getPixel(vec2i pos) const {
    assert_pos(pos);

    return pixels_.at(static_cast<size_t>(pos.y * size_.x + pos.x));
}

void Layer::setPixel(vec2i pos, Color pixel) {
    assert_pos(pos);

    pixels_.at(static_cast<size_t>(pos.y * size_.x + pos.x)) = pixel;
}

void Layer::assert_pos(vec2i pos) const {
    if(!( 0 < pos.x && pos.x < size_.x &&
          0 < pos.y && pos.x < size_.y    )) {
        LOG_F(FATAL, "Invalid position: %d %d", pos.x, pos.y);
        assert(0);
    }
}

void Layer::assert_size(vec2i size) const {
    if (size.x < 0 || size.y < 0 || size.x > kMaxWidth || size.y > kMaxHeight) {
        LOG_F(FATAL, "Invalid size: %d %d", size.x, size.y);
        assert(0);
    }
}

void Layer::changeSize(vec2i new_size) {
    std::vector<Color> new_pixels(static_cast<size_t>(new_size.x) *
                                   static_cast<size_t>(new_size.y));

    for (int x = 0; x < size_.x; x++) {
        for (int y = 0; y < size_.y; y++) {
            new_pixels.at(static_cast<size_t>(y * new_size.x + x)) =
                pixels_.at(static_cast<size_t>(y * size_.x + x));
        }
    }

    size_ = new_size;
    pixels_ = std::move(new_pixels);
}

// Canvas implementation

Canvas::Canvas(vec2i size)
    : temp_layer_(std::make_unique<Layer>(size)),
      layers_(1),
      size_(size),
      texture_(),
      sprite_(),
      last_mouse_pos_({0, 0}),
      is_pressed_(false) {
    texture_.create(static_cast<unsigned int>(size_.x),
                    static_cast<unsigned int>(size_.y));

    layers_.push_back(std::make_unique<Layer>(size_));
}

void Canvas::draw(psapi::ARenderWindow* renderWindow) { 
    for (const auto& layer : layers_) {
        texture_.update(layer->pixels_.data());
        sprite_.setTexture(&texture_);
        renderWindow->draw(&sprite_);
    }

    texture_.update(temp_layer_->pixels_.data());
    sprite_.setTexture(&texture_);
    renderWindow->draw(&sprite_);
}

void Canvas::DrawLayer(const Layer& layer, psapi::ARenderWindow* renderWindow) {
    texture_.update(layer.pixels_.data());
    sprite_.setTexture(&texture_);
    sprite_.setScale(scale_.x, scale_.y);
    renderWindow->draw(&sprite_);
}

psapi::ILayer* Canvas::getLayer(size_t index) {
    return layers_.at(index).get();
}

const psapi::ILayer* Canvas::getLayer(size_t index) const {
    return layers_.at(index).get();
}

psapi::ILayer* Canvas::getTempLayer() {
    return temp_layer_.get();
}

const psapi::ILayer* Canvas::getTempLayer() const {
    return temp_layer_.get();
}

void Canvas::cleanTempLayer() {
    Color pixel = {0u, 0u, 0u, 255u};
    for (int x = 0; x < size_.x; x++) {
        for (int y = 0; y < size_.y; y++) {
            temp_layer_->setPixel({x, y}, pixel);
        }
    }
}

size_t Canvas::getNLayers() const {
    return layers_.size();
}

bool Canvas::removeLayer(size_t index) {
    if (index >= layers_.size()) {
        LOG_S(WARNING) << "Invalid layer index: " << index;
        return false;
    }

    layers_.erase(layers_.begin() + (long)index);
    return true;
}

bool Canvas::insertLayer(size_t index, std::unique_ptr<psapi::ILayer> layer) {
    if (index > layers_.size()) {
        LOG_S(WARNING) << "Invalid layer index: " << index;
        return false;
    }

    std::unique_ptr<Layer> new_layer = std::make_unique<Layer>(size_);
    for (int x = 0; x < size_.x; x++) {
        for (int y = 0; y < size_.y; y++) {
            new_layer->setPixel({x, y}, layer->getPixel({x, y}));
        }
    }

    layers_.insert(layers_.begin() + (long)index, std::move(new_layer));
    return true;
}

bool Canvas::insertEmptyLayer(size_t index) {
    if (index > layers_.size()) {
        LOG_S(WARNING) << "Invalid layer index: " << index;
        return false;
    }

    layers_.insert(layers_.begin() + (long)index, std::make_unique<Layer>(size_));
    return true;
}

void Canvas::setPos(vec2i pos) {
    pos_ = pos;
}

void Canvas::setSize(vec2i size) {
    size_ = size;

    temp_layer_->changeSize(size);
    for (auto& layer : layers_) {
        layer->changeSize(size);
    }
}

void Canvas::setScale(vec2f scale) {
    scale_ = scale;
}

size_t Canvas::getActiveLayerIndex() const {
    return active_layer_;
}

void Canvas::setActiveLayerIndex(size_t index) {
    if (index >= layers_.size()) {
        LOG_S(WARNING) << "Invalid layer index: " << index;
        return;
    }

    active_layer_ = index;
}
