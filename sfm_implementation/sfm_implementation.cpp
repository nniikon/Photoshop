#include "sfm_implementation.h"
#include <SFML/Graphics.hpp>
#include <cassert>
#include <memory>

namespace psapi
{

namespace sfm
{

// RenderWindow implementation
RenderWindow::RenderWindow(unsigned int width, unsigned int height, const std::string& title)
    : window_(sf::VideoMode(width, height), title) {
    window_.setFramerateLimit(60);
}

bool RenderWindow::isOpen() const {
    return window_.isOpen();
}

void RenderWindow::clear() {
    window_.clear();
}

void RenderWindow::display() {
    window_.display();
}

void RenderWindow::close() {
    window_.close();
}

vec2u RenderWindow::getSize() const {
    return vec2u(window_.getSize().x, window_.getSize().y);
}

bool RenderWindow::pollEvent(Event& event) {
    sf::Event sfEvent;
    if (!window_.pollEvent(sfEvent))
    {
        return false;
    }

    switch (sfEvent.type)
    {
        case sf::Event::Closed:
            event.type = Event::Closed;
            break;

        case sf::Event::Resized:
            event.type = Event::Resized;
            event.size.width  = sfEvent.size.width;
            event.size.height = sfEvent.size.height;
            break;

        case sf::Event::LostFocus:
            event.type = Event::LostFocus;
            break;

        case sf::Event::GainedFocus:
            event.type = Event::GainedFocus;
            break;

        case sf::Event::TextEntered:
            event.type = Event::TextEntered;
            event.text.unicode = sfEvent.text.unicode;
            break;

        case sf::Event::KeyPressed:
            event.type = Event::KeyPressed;
            event.key.code = static_cast<Keyboard::Key>(sfEvent.key.code);
            event.key.alt     = sfEvent.key.alt;
            event.key.control = sfEvent.key.control;
            event.key.shift   = sfEvent.key.shift;
            event.key.system  = sfEvent.key.system;
            break;

        case sf::Event::KeyReleased:
            event.type = Event::KeyReleased;
            event.key.code = static_cast<Keyboard::Key>(sfEvent.key.code);
            event.key.alt     = sfEvent.key.alt;
            event.key.control = sfEvent.key.control;
            event.key.shift   = sfEvent.key.shift;
            event.key.system  = sfEvent.key.system;
            break;

        case sf::Event::MouseWheelScrolled:
            event.type = Event::MouseWheelScrolled;
            event.mouseWheel.delta = sfEvent.mouseWheelScroll.delta;
            event.mouseWheel.x     = sfEvent.mouseWheelScroll.x;
            event.mouseWheel.y     = sfEvent.mouseWheelScroll.y;
            break;

        case sf::Event::MouseButtonPressed:
            event.type = Event::MouseButtonPressed;
            event.mouseButton.button = static_cast<Mouse::Button>(sfEvent.mouseButton.button);
            event.mouseButton.x = sfEvent.mouseButton.x;
            event.mouseButton.y = sfEvent.mouseButton.y;
            break;

        case sf::Event::MouseButtonReleased:
            event.type = Event::MouseButtonReleased;
            event.mouseButton.button = static_cast<Mouse::Button>(sfEvent.mouseButton.button);
            event.mouseButton.x = sfEvent.mouseButton.x;
            event.mouseButton.y = sfEvent.mouseButton.y;
            break;

        case sf::Event::MouseMoved:
            event.type = Event::MouseMoved;
            event.mouseMove.x = sfEvent.mouseMove.x;
            event.mouseMove.y = sfEvent.mouseMove.y;
            break;

        case sf::Event::MouseEntered:
            event.type = Event::MouseEntered;
            break;

        case sf::Event::MouseLeft:
            event.type = Event::MouseLeft;
            break;

        case sf::Event::MouseWheelMoved: // deprecated
        case sf::Event::JoystickButtonPressed:
        case sf::Event::JoystickButtonReleased:
        case sf::Event::JoystickMoved:
        case sf::Event::JoystickConnected:
        case sf::Event::JoystickDisconnected:
        case sf::Event::TouchBegan:
        case sf::Event::TouchMoved:
        case sf::Event::TouchEnded:
        case sf::Event::SensorChanged:
        case sf::Event::Count:
        default:
            event.type = Event::Unknown;
            break;
    }

    return true;
}

void RenderWindow::draw(Drawable *target) {
    if (target) {
        target->draw(this);
    }
}

void RenderWindow::setFps(float fps) {
    window_.setFramerateLimit(static_cast<unsigned int>(fps));
}

float RenderWindow::getFps() const {
    return 0.f;
}

std::unique_ptr<IRenderWindow> RenderWindow::create(unsigned int width, unsigned int height, const std::string& title) {
    return std::make_unique<RenderWindow>(width, height, title);
}

// FUCK YOU VANYA (IImage) FUCK YOU FUCK YOU

void IImage::create(unsigned int width, unsigned int height, const Color &color) { assert(0); }
void IImage::create(vec2u size,                              const Color &color) { assert(0); }

void IImage::create(unsigned int width, unsigned int height, const Color *pixels) { assert(0); }
void IImage::create(vec2u size,                              const Color *pixels) { assert(0); }

bool IImage::loadFromFile(const std::string &filename) { assert(0); return false; }

vec2u IImage::getSize() const { return vec2u(); }
void IImage::setPixel(unsigned int x, unsigned int y, const Color &color) { assert(0); }
void IImage::setPixel(vec2u pos, const Color &color) { assert(0); }

Color IImage::getPixel(unsigned int x, unsigned int y) const { assert(0); return Color(); }
Color IImage::getPixel(vec2u pos) const { assert(0); return Color(); }

// FUCK YOU VANYA (PixelsArray)

void PixelsArray::setColor(const Color &color, size_t ind) {}
Color PixelsArray::getColor(size_t ind) const { return Color(); }

void PixelsArray::setPosition(const vec2i &coord, size_t ind) { assert(0); }
void PixelsArray::setPosition(const vec2f &coord, size_t ind) { assert(0); }
void PixelsArray::setPosition(const vec2d &coord, size_t ind) { assert(0); }

void PixelsArray::setPosition(int    x, int    y, size_t ind) { assert(0); }
void PixelsArray::setPosition(float  x, float  y, size_t ind) { assert(0); }
void PixelsArray::setPosition(double x, double y, size_t ind) { assert(0); }

void PixelsArray::draw(IRenderWindow *window) const { assert(0); }

std::unique_ptr<IPixelsArray> create() {
    return std::make_unique<PixelsArray>();
}

// FUCK YOU VANYA (Image)

void Image::create(unsigned int width, unsigned int height, const Color &color) {
    sf::Color sfColor(color.r, color.g, color.b, color.a);
    image_.create(width, height, sfColor);
}

void Image::create(vec2u size, const Color &color) {
    create(size.x, size.y, color);
}

void Image::create(unsigned int width, unsigned int height, const Color *pixels) {
    image_.create(width, height, reinterpret_cast<const sf::Uint8*>(pixels));
}

void Image::create(vec2u size, const Color *pixels) {
    create(size.x, size.y, pixels);
}

bool Image::loadFromFile(const std::string &filename) {
    return image_.loadFromFile(filename);
}

vec2u Image::getSize() const {
    sf::Vector2u size = image_.getSize();
    return vec2u{size.x, size.y};
}

void Image::setPixel(unsigned int x, unsigned int y, const Color &color) {
    sf::Color sfColor(color.r, color.g, color.b, color.a);
    image_.setPixel(x, y, sfColor);
}

void Image::setPixel(vec2u pos, const Color &color) {
    setPixel(pos.x, pos.y, color);
}

Color Image::getPixel(unsigned int x, unsigned int y) const {
    sf::Color sfColor = image_.getPixel(x, y);
    return Color(sfColor.r, sfColor.g, sfColor.b, sfColor.a);
}

Color Image::getPixel(vec2u pos) const {
    return getPixel(pos.x, pos.y);
}

std::unique_ptr<IImage> Image::create() {
    return std::make_unique<Image>();
}

// Texture implementation

Texture::Texture()
{}

bool Texture::create(unsigned int width, unsigned int height) {
    return texture_.create(width, height);
}

bool Texture::loadFromFile(const std::string& filename, const IntRect& area) {
    sf::IntRect sf_area(area.top_x, area.top_y, area.width, area.height);
    return texture_.loadFromFile(filename, sf_area);
}

bool Texture::loadFromMemory(const void* data, std::size_t size, const IntRect& area) {
    sf::IntRect sf_area(area.top_x, area.top_y, area.width, area.height);
    return texture_.loadFromMemory(data, size, sf_area);
}

vec2u Texture::getSize() const {
    sf::Vector2u size = texture_.getSize();
    return {size.x, size.y};
}

std::unique_ptr<IImage> Texture::copyToImage() const {
    sf::Image sfImage = texture_.copyToImage();

    auto image = std::make_unique<Image>();

    vec2u size = {sfImage.getSize().x, sfImage.getSize().y};
    image->create(size, reinterpret_cast<const Color*>(sfImage.getPixelsPtr()));

    return image;
}


void Texture::update(const IImage *image) {
    try {
        auto my_image = dynamic_cast<const Image*>(image);
        texture_.update(my_image->image_);
    }
    catch (...) {
        assert(0); // Shouldn't happen
    }
}

void Texture::update(const Color *pixels) {
    vec2u size = getSize();
    texture_.update(reinterpret_cast<const sf::Uint8*>(pixels), size.x, size.y, 0, 0);
}

void Texture::update(const Color *pixels, unsigned int width, unsigned int height, unsigned int x, unsigned int y) {
    texture_.update(reinterpret_cast<const sf::Uint8*>(pixels), width, height, x, y);
}

std::unique_ptr<ITexture> Texture::create() {
    return std::make_unique<Texture>();
}

// Sprite implementation

void Sprite::setTexture(const ITexture *texture, bool reset_rect) {
    const Texture* sfmTexture = static_cast<const Texture*>(texture);
    sprite_.setTexture(sfmTexture->texture_, reset_rect);
}

void Sprite::setTextureRect(const IntRect &rectangle) {
    sprite_.setTextureRect(sf::IntRect(static_cast<int>(rectangle.top_x),
                                       static_cast<int>(rectangle.top_y),
                                       static_cast<int>(rectangle.width),
                                       static_cast<int>(rectangle.height)));
}

void Sprite::setPosition(float x, float y) {
    sprite_.setPosition(x, y);
}

void Sprite::setPosition(const vec2f &pos) {
    sprite_.setPosition(pos.x, pos.y);
}

void Sprite::setScale(float factorX, float factorY) {
    sprite_.setScale(factorX, factorY);
}

vec2u Sprite::getSize() const {
    return {static_cast<unsigned int>(sprite_.getGlobalBounds().getSize().x),
            static_cast<unsigned int>(sprite_.getGlobalBounds().getSize().y)};
}

void Sprite::setColor(const Color &color) {
    sprite_.setColor(sf::Color(color.r, color.g, color.b, color.a));
}

Color Sprite::getColor() const {
    return { sprite_.getColor().r,
             sprite_.getColor().g,
             sprite_.getColor().b,
             sprite_.getColor().a };
}

void Sprite::setRotation(float angle) {
    sprite_.setRotation(angle);
}

const vec2f Sprite::getPosition() const
{
    sf::Vector2f pos = sprite_.getPosition();
    return { pos.x, pos.y };
}

IntRect Sprite::getGlobalBounds() const {
    sf::FloatRect bounds = sprite_.getGlobalBounds();
    return { static_cast<int>(bounds.left),  static_cast<int>(bounds.top),
             static_cast<int>(bounds.width), static_cast<int>(bounds.height) };
}

void Sprite::draw(IRenderWindow *window) const {
    RenderWindow* sfmWindow = static_cast<RenderWindow*>(window);
    sfmWindow->window_.draw(sprite_);
}

std::unique_ptr<psapi::sfm::ISprite> ISprite::create() {
    return std::make_unique<Sprite>();
}

// Font implementation

bool Font::loadFromFile(const std::string& filename) {
    return font_.loadFromFile(filename);
}

// Text implementation

void Text::draw(IRenderWindow *window) const {
    RenderWindow* sfmWindow = static_cast<RenderWindow*>(window);
    sfmWindow->window_.draw(text_);
}

void Text::setString(const std::string& string) {
    text_.setString(string);
}

void Text::setFont(const IFont* font) {
    const Font* sfmFont = static_cast<const Font*>(font);
    text_.setFont(sfmFont->font_);
}

void Text::setCharacterSize(unsigned int size) {
    text_.setCharacterSize(size);
}

void Text::setStyle(uint32_t style) {
    text_.setStyle(style);
}

void Text::setFillColor(const Color* color) {
    text_.setFillColor(sf::Color(color->r, color->g, color->b, color->a));
}

void Text::setOutlineColor(const Color* color) {
    text_.setOutlineColor(sf::Color(color->r, color->g, color->b, color->a));
}

void Text::setOutlineThickness(float thickness) {
    text_.setOutlineThickness(thickness);
}

// Mouse implementation

vec2i Mouse::getPosition() {
    sf::Vector2i pos = sf::Mouse::getPosition();
    return { pos.x, pos.y };
}

vec2i Mouse::getPosition(const IRenderWindow* window) {
    const RenderWindow* sfmWindow = static_cast<const RenderWindow*>(window);
    sf::Vector2i pos = sf::Mouse::getPosition(sfmWindow->window_);
    return { pos.x, pos.y };
}

// Rectangle implementation

RectangleShape::RectangleShape(unsigned int width, unsigned int height)
    : shape(sf::Vector2f(static_cast<float>(width),
                         static_cast<float>(height))) {}

RectangleShape::RectangleShape(const vec2u &size)
    : shape(sf::Vector2f(static_cast<float>(size.x), 
                         static_cast<float>(size.y))) {}

void RectangleShape::draw(IRenderWindow *window) const {
    auto sfmWindow = static_cast<RenderWindow*>(window);
    sfmWindow->window_.draw(shape);
}

void RectangleShape::setTexture(const ITexture *texture) {
    const auto sfTexture = dynamic_cast<const sf::Texture*>(texture);
    if (sfTexture) {
        shape.setTexture(sfTexture);
        imageNeedsUpdate = true;
    }
}

void RectangleShape::setFillColor(const Color &color) {
    shape.setFillColor(sf::Color(color.r, color.g, color.b, color.a));
    imageNeedsUpdate = true;
}

void RectangleShape::setPosition(const vec2i &pos) {
    shape.setPosition(sf::Vector2f(static_cast<float>(pos.x),
                                   static_cast<float>(pos.y)));
    imageNeedsUpdate = true;
}

void RectangleShape::setPosition(const vec2f &pos) {
    shape.setPosition(sf::Vector2f(pos.x, pos.y));
    imageNeedsUpdate = true;
}

void RectangleShape::setPosition(const vec2d &pos) {
    shape.setPosition(sf::Vector2f(static_cast<float>(pos.x), static_cast<float>(pos.y)));
    imageNeedsUpdate = true;
}

void RectangleShape::setScale(const vec2f &scale) {
    shape.setScale(sf::Vector2f(scale.x, scale.y));
    imageNeedsUpdate = true;
}

void RectangleShape::setSize(const vec2u &size) {
    shape.setSize(sf::Vector2f(static_cast<float>(size.x),
                               static_cast<float>(size.y)));
    imageNeedsUpdate = true;
}

void RectangleShape::setRotation(float angle) {
    shape.setRotation(angle);
    imageNeedsUpdate = true;
}

void RectangleShape::setOutlineColor(const Color &color) {
    shape.setOutlineColor(sf::Color(color.r, color.g, color.b, color.a));
    imageNeedsUpdate = true;
}

void RectangleShape::setOutlineThickness(float thickness) {
    shape.setOutlineThickness(thickness);
    imageNeedsUpdate = true;
}

float RectangleShape::getRotation() const {
    return shape.getRotation();
}

vec2f RectangleShape::getScale() const {
    auto scale = shape.getScale();
    return {scale.x, scale.y};
}

vec2f RectangleShape::getPosition() const {
    auto pos = shape.getPosition();
    return {pos.x, pos.y};
}

const Color &RectangleShape::getFillColor() const {
    return reinterpret_cast<const Color&>(shape.getFillColor());
}

vec2u RectangleShape::getSize() const {
    auto size = shape.getSize();
    return {static_cast<unsigned int>(size.x), static_cast<unsigned int>(size.y)};
}

float RectangleShape::getOutlineThickness() const {
    return shape.getOutlineThickness();
}

const Color &RectangleShape::getOutlineColor() const {
    return reinterpret_cast<const Color&>(shape.getOutlineColor());
}

const IImage *RectangleShape::getImage() const {
    if (imageNeedsUpdate) {
        updateImage();
    }
    return cachedImage.get();
}

void RectangleShape::move(const vec2f &offset) {
    shape.move(sf::Vector2f(offset.x, offset.y));
    imageNeedsUpdate = true;
}

void RectangleShape::updateImage() const {
    sf::RenderTexture renderTexture;
    renderTexture.create(getSize().x, getSize().y);
    renderTexture.clear(sf::Color::Transparent);
    renderTexture.draw(shape);
    renderTexture.display();

    sf::Image image = renderTexture.getTexture().copyToImage();
    cachedImage = std::make_unique<Image>();
    cachedImage->create(image.getSize().x,
                        image.getSize().y,
                        reinterpret_cast<const Color*>(image.getPixelsPtr()));
    imageNeedsUpdate = false;
}

// Ellipse implementation

EllipseShape::EllipseShape(unsigned int width, unsigned int height)
    : shape(static_cast<float>(width) / 2.0f) {
    setSize({width, height});
}

EllipseShape::EllipseShape(const vec2u &size)
    : EllipseShape(size.x, size.y) {}

EllipseShape::EllipseShape(unsigned int radius)
    : shape(static_cast<float>(radius)) {}

void EllipseShape::draw(IRenderWindow *window) const {
    auto sfmWindow = static_cast<RenderWindow*>(window);
    sfmWindow->window_.draw(shape);
}

void EllipseShape::setTexture(const ITexture *texture) {
    const auto sfTexture = dynamic_cast<const sf::Texture*>(texture);
    if (sfTexture) {
        shape.setTexture(sfTexture);
        imageNeedsUpdate = true;
    }
}

void EllipseShape::setFillColor(const Color &color) {
    shape.setFillColor(sf::Color(color.r, color.g, color.b, color.a));
    imageNeedsUpdate = true;
}

void EllipseShape::setPosition(const vec2i &pos) {
    shape.setPosition(sf::Vector2f(static_cast<float>(pos.x),
                                   static_cast<float>(pos.y)));
    imageNeedsUpdate = true;
}

void EllipseShape::setPosition(const vec2f &pos) {
    shape.setPosition(sf::Vector2f(pos.x, pos.y));
    imageNeedsUpdate = true;
}

void EllipseShape::setPosition(const vec2d &pos) {
    shape.setPosition(sf::Vector2f(static_cast<float>(pos.x), static_cast<float>(pos.y)));
    imageNeedsUpdate = true;
}

void EllipseShape::setScale(const vec2f &scale) {
    shape.setScale(sf::Vector2f(scale.x, scale.y));
    imageNeedsUpdate = true;
}

void EllipseShape::setSize(const vec2u &size) {
    // Set the radius to half the width, and adjust scale to simulate an ellipse
    shape.setRadius(static_cast<float>(size.x) / 2.0f);
    shape.setScale(1.0f, static_cast<float>(size.y) / static_cast<float>(size.x));
    imageNeedsUpdate = true;
}

void EllipseShape::setRotation(float angle) {
    shape.setRotation(angle);
    imageNeedsUpdate = true;
}

void EllipseShape::setOutlineColor(const Color &color) {
    shape.setOutlineColor(sf::Color(color.r, color.g, color.b, color.a));
    imageNeedsUpdate = true;
}

void EllipseShape::setOutlineThickness(float thickness) {
    shape.setOutlineThickness(thickness);
    imageNeedsUpdate = true;
}

float EllipseShape::getRotation() const {
    return shape.getRotation();
}

vec2f EllipseShape::getScale() const {
    auto scale = shape.getScale();
    return {scale.x, scale.y};
}

vec2f EllipseShape::getPosition() const {
    auto pos = shape.getPosition();
    return {pos.x, pos.y};
}

const Color &EllipseShape::getFillColor() const {
    return reinterpret_cast<const Color&>(shape.getFillColor());
}

vec2u EllipseShape::getSize() const {
    auto radius = shape.getRadius();
    return {static_cast<unsigned int>(radius * 2.0f * shape.getScale().x),
            static_cast<unsigned int>(radius * 2.0f * shape.getScale().y)};
}

float EllipseShape::getOutlineThickness() const {
    return shape.getOutlineThickness();
}

const Color &EllipseShape::getOutlineColor() const {
    return reinterpret_cast<const Color&>(shape.getOutlineColor());
}

const IImage *EllipseShape::getImage() const {
    if (imageNeedsUpdate) {
        updateImage();
    }
    return cachedImage.get();
}

void EllipseShape::move(const vec2f &offset) {
    shape.move(sf::Vector2f(offset.x, offset.y));
    imageNeedsUpdate = true;
}

void EllipseShape::updateImage() const {
    sf::RenderTexture renderTexture;
    auto size = getSize();
    renderTexture.create(size.x, size.y);
    renderTexture.clear(sf::Color::Transparent);
    renderTexture.draw(shape);
    renderTexture.display();

    sf::Image image = renderTexture.getTexture().copyToImage();
    cachedImage = std::make_unique<Image>();
    cachedImage->create(image.getSize().x,
                        image.getSize().y,
                        reinterpret_cast<const Color*>(image.getPixelsPtr()));
    imageNeedsUpdate = false;
}

} // namespace sfm

} // namespace psapi
