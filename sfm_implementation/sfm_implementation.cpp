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
    window_.setFramerateLimit(10);
}

bool RenderWindow::isOpen() {
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

void PixelsArray::draw(IRenderWindow *window) { assert(0); }

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

void Image::draw(IRenderWindow *window) {
    assert(0); // FU VANYA
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

vec2i Sprite::getSize() const {
    return {static_cast<int>(sprite_.getGlobalBounds().getSize().x),
            static_cast<int>(sprite_.getGlobalBounds().getSize().y)};
}

void Sprite::setColor(const Color &color) {
    sprite_.setColor(sf::Color(color.r, color.g, color.b, color.a));
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

void Sprite::draw(IRenderWindow *window) {
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

void Text::draw(IRenderWindow *window) {
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

} // namespace sfm

} // namespace psapi
