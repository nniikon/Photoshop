#include "ps_canvas.h"

#define LOGURU_WITH_STREAMS 1
#include "loguru.hpp"

#include "sfm_implementation.h"
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
    if(!( 0 <= pos.x && pos.x < size_.x &&
          0 <= pos.y && pos.x < size_.y    )) {
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

Canvas::Canvas(vec2i size, vec2i pos)
    : temp_layer_(std::make_unique<Layer>(size)),
      layers_(0),
      pos_(pos),
      size_(size),
      scale_{1, 1},
      last_mouse_pos_({0, 0}),
      is_pressed_(false),
      texture_(),
      sprite_(),
      is_active_(true) {
    texture_ = psapi::sfm::Texture::create();
    texture_->create(static_cast<unsigned int>(size_.x),
                     static_cast<unsigned int>(size_.y));
    sprite_  = psapi::sfm::Sprite::create();

    layers_.push_back(std::make_unique<Layer>(size_));

    for (int x = 0; x < size_.x; x++) {
        for (int y = 0; y < size_.y; y++) {
            temp_layer_->setPixel({x, y}, {255, 255, 255, 0});
        }
    }
    for (int x = 0; x < size_.x; x++) {
        for (int y = 0; y < size_.y; y++) {
            layers_.back()->setPixel({x, y}, {255, 255, 255, 255});
        }
    }
}

void Canvas::DrawLayer(const Layer& layer, psapi::IRenderWindow* renderWindow) {
    texture_->update(layer.pixels_.data());
    sprite_->setTexture(texture_.get());
    sprite_->setScale(scale_.x, scale_.y);
    sprite_->setPosition(static_cast<float>(pos_.x),
                         static_cast<float>(pos_.y));

    sprite_->draw(renderWindow);
}

void Canvas::draw(psapi::IRenderWindow* renderWindow) { 
    if (!is_active_) {
        return;
    }

    for (const auto& layer : layers_) {
        DrawLayer(*layer, renderWindow);
    }

    DrawLayer(*temp_layer_, renderWindow);
}

bool Canvas::setLastMousePos(const psapi::IRenderWindow* renderWindow) {
    vec2i mouse_pos = psapi::sfm::Mouse::getPosition(renderWindow);
    
    bool is_in_window = pos_.x <= mouse_pos.x && mouse_pos.x < pos_.x + size_.x &&
                        pos_.y <= mouse_pos.y && mouse_pos.y < pos_.y + size_.y;

    last_mouse_pos_ = {mouse_pos.x - pos_.x, 
                       mouse_pos.y - pos_.y};

    return is_in_window;
}

bool Canvas::update(const psapi::IRenderWindow* renderWindow,
                    const psapi::sfm::Event& event) {
    if (!is_active_) {
        return false;
    }

    if (event.type == psapi::sfm::Event::MouseButtonReleased) {
        is_pressed_ = false;
    }

    bool is_in_window = setLastMousePos(renderWindow);
    if (!is_in_window) {
        return false;
    }

    if (event.type == psapi::sfm::Event::MouseButtonPressed) {
        is_pressed_ = true;
    }

    return false;
}

psapi::wid_t Canvas::getId() const {
    return psapi::kCanvasWindowId;
}

const psapi::IWindow* Canvas::getWindowById(psapi::wid_t id) const {
    if (id == psapi::kCanvasWindowId) {
        return this;
    }
    return nullptr;
}

psapi::IWindow* Canvas::getWindowById(psapi::wid_t id) {
    return const_cast<psapi::IWindow*>(static_cast<const Canvas*>(this)->getWindowById(id));
}

vec2i Canvas::getPos() const {
    return pos_;
}

vec2u Canvas::getSize() const {
    vec2u size = {static_cast<unsigned int>(size_.x),
                  static_cast<unsigned int>(size_.y)};
    return size;
}

void Canvas::setParent(const IWindow* parent) {
    return;
}

void Canvas::forceActivate() {
    is_active_ = true;
}

void Canvas::forceDeactivate() {
    is_active_ = false;
}

bool Canvas::isWindowContainer() const {
    return false;
}


psapi::ILayer* Canvas::getLayer(size_t index) {
    return const_cast<psapi::ILayer*>(const_cast<const Canvas*>(this)->getLayer(index));
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

size_t Canvas::getNumLayers() const {
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

vec2i Canvas::getMousePosition() const {
    return last_mouse_pos_;
}

bool Canvas::isPressed() const {
    return is_pressed_;
}

bool Canvas::isActive() const {
    return is_active_;
}
