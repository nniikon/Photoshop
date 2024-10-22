#include "sfm_implementation.h"
#include <SFML/Graphics.hpp>
#include <memory>

namespace psapi
{

namespace sfm
{

// RenderWindow implementation
RenderWindow::RenderWindow(unsigned int width, unsigned int height, const std::string& title)
    : window_(sf::VideoMode(width, height), title) {}

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

// fuck it
void FuckYouVanyaImage::setColor(const Color &color, size_t ind) {}
void FuckYouVanyaImage::setPosition(const vec2i &coord, size_t ind) {}
void FuckYouVanyaImage::setPosition(const vec2f &coord, size_t ind) {}
void FuckYouVanyaImage::setPosition(const vec2d &coord, size_t ind) {}
void FuckYouVanyaImage::setPosition(int x, int y, size_t ind) {}
void FuckYouVanyaImage::setPosition(float x, float y, size_t ind) {}
void FuckYouVanyaImage::setPosition(double x, double y, size_t ind) {}
void FuckYouVanyaImage::draw(ARenderWindow *window) {}

bool Texture::create(unsigned int width, unsigned int height) {
    return texture_.create(width, height);
}

bool Texture::loadFromFile(const std::string& filename, const IntRect& area) {
    sf::IntRect sfArea(static_cast<int>(area.top_x),
                       static_cast<int>(area.top_y),
                       static_cast<int>(area.width),
                       static_cast<int>(area.height));
    return texture_.loadFromFile(filename, sfArea);
}

bool Texture::loadFromMemory(const void* data, std::size_t size, const IntRect& area) {
    sf::IntRect sfArea(static_cast<int>(area.top_x),
                       static_cast<int>(area.top_y),
                       static_cast<int>(area.width),
                       static_cast<int>(area.height));
    return texture_.loadFromMemory(data, size, sfArea);
}

bool Texture::loadFromImage(const AImage *image, const IntRect& area) {
    const FuckYouVanyaImage* sfmImage = static_cast<const FuckYouVanyaImage*>(image);
    sf::IntRect sfArea(static_cast<int>(area.top_x),
                       static_cast<int>(area.top_y),
                       static_cast<int>(area.width),
                       static_cast<int>(area.height));
    return texture_.loadFromImage(sfmImage->image_, sfArea);
}

vec2u Texture::getSize() const {
    return { texture_.getSize().x, texture_.getSize().y };
}

std::unique_ptr<AImage> Texture::copyToImage() const {
    auto image = std::make_unique<FuckYouVanyaImage>();
    return image;
}

void Texture::update(const Color *pixels) {
    texture_.update(reinterpret_cast<const sf::Uint8*>(pixels));
}

void Texture::update(const Color *pixels, unsigned int width, unsigned int height, unsigned int x, unsigned int y) {
    texture_.update(reinterpret_cast<const sf::Uint8*>(pixels), width, height, x, y);
}

ATexture& Texture::operator=(const ATexture& right) {
    if (this != &right)
    {
        const Texture* sfmTexture = dynamic_cast<const Texture*>(&right);
        texture_ = sfmTexture->texture_;
    }
    return *this;
}

void Sprite::setTexture(const ATexture *texture, bool reset_rect) {
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
    return { static_cast<unsigned int>(bounds.left), static_cast<unsigned int>(bounds.top),
             static_cast<unsigned int>(bounds.width), static_cast<unsigned int>(bounds.height) };
}

void Sprite::draw(ARenderWindow *window) {
    RenderWindow* sfmWindow = static_cast<RenderWindow*>(window);
    sfmWindow->window_.draw(sprite_);
}

bool Font::loadFromFile(const std::string& filename) {
    return font_.loadFromFile(filename);
}

void Text::draw(ARenderWindow *window) {
    RenderWindow* sfmWindow = static_cast<RenderWindow*>(window);
    sfmWindow->window_.draw(text_);
}

void Text::setString(const std::string& string) {
    text_.setString(string);
}

void Text::setFont(const AFont* font) {
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

} // namespace sfm

} // namespace psapi
