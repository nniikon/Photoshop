#ifndef SFM_IMPLEMENTATION_HPP
#define SFM_IMPLEMENTATION_HPP

#include "../photoshopAPI/api_sfm.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace psapi
{
namespace sfm
{

class RenderWindow : public ARenderWindow
{
private:
    sf::RenderWindow window_;

    friend class Text;
    friend class Sprite;

public:
    RenderWindow(unsigned int width, unsigned int height, const std::string& title);
    virtual ~RenderWindow() = default;

    bool isOpen() override;
    void clear() override;
    void display() override;
    void close() override;
    bool pollEvent(Event& event) override;
    void draw(Drawable *target) override;
};

class FuckYouVanyaImage : public AImage
{
private:
    sf::Image image_;

    friend class Texture;

public:
    FuckYouVanyaImage() = default;
    virtual ~FuckYouVanyaImage() = default;

    void setColor(const Color &color, size_t ind) override;

    void setPosition(const vec2i &coord, size_t ind) override;
    void setPosition(const vec2f &coord, size_t ind) override;
    void setPosition(const vec2d &coord, size_t ind) override;
    void setPosition(int x, int y, size_t ind) override;
    void setPosition(float x, float y, size_t ind) override;
    void setPosition(double x, double y, size_t ind) override;
    void draw(ARenderWindow *window) override;
};

class Texture : public ATexture
{
private:
    sf::Texture texture_;

    friend class Sprite;

public:
    bool create(unsigned int width, unsigned int height) override;
    bool loadFromFile(const std::string& filename, const IntRect& area = IntRect()) override;
    bool loadFromMemory(const void* data, std::size_t size, const IntRect& area = IntRect()) override;
    bool loadFromImage(const AImage *image, const IntRect& area = IntRect()) override;
    vec2u getSize() const override;
    std::unique_ptr<AImage> copyToImage() const override;
    void update(const Color *pixels) override;
    void update(const Color *pixels, unsigned int width, unsigned int height, unsigned int x, unsigned int y) override;
    ATexture& operator=(const ATexture& right) override;
};

class Sprite : public ASprite
{
private:
    sf::Sprite sprite_;

public:
    virtual ~Sprite() = default;

    void setTexture(const ATexture *texture, bool reset_rect = false) override;
    void setTextureRect(const IntRect &rectangle) override;
    void setPosition(float x, float y) override;
    void setPosition(const vec2f &pos) override;
    void setScale(float factorX, float factorY) override;
    void setColor(const Color &color) override;
    void setRotation(float angle) override;
    const vec2f getPosition() const override;
    IntRect getGlobalBounds() const override;
    void draw(ARenderWindow *window) override;
};

class Font : public AFont
{
private:
    sf::Font font_;

    friend class Text;

public:
    bool loadFromFile(const std::string& filename) override;
};

class Text : public AText
{
private:
    sf::Text text_;

    friend class RenderWindow;

public:
    virtual ~Text() = default;

    void draw(ARenderWindow *window) override;
    void setString(const std::string& string) override;
    void setFont(const AFont* font) override;
    void setCharacterSize(unsigned int size) override;
    void setStyle(uint32_t style) override;
    void setFillColor(const Color* color) override;
    void setOutlineColor(const Color* color) override;
    void setOutlineThickness(float thickness) override;
};

} // namespace sfm
} // namespace psapi

#endif // SFM_IMPLEMENTATION_HPP

