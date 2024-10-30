#ifndef SFM_IMPLEMENTATION_HPP
#define SFM_IMPLEMENTATION_HPP

#include "../photoshopAPI/api_sfm.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace psapi
{

namespace sfm
{

class RenderWindow : public IRenderWindow {
private:
    sf::RenderWindow window_;

    friend class Text;
    friend class Sprite;
    friend class psapi::sfm::Mouse;

public:
    RenderWindow(unsigned int width, unsigned int height, const std::string& title);
    virtual ~RenderWindow() = default;

    bool isOpen()                override;
    void clear()                 override;
    void display()               override;
    void close()                 override;
    vec2u getSize() const        override;
    bool pollEvent(Event& event) override;

    void draw(Drawable *target) override;

    static std::unique_ptr<IRenderWindow>
        create(unsigned int width, unsigned int height, const std::string& name);
};

class PixelsArray : public IPixelsArray {
public:
    virtual ~PixelsArray() = default;

    virtual void setColor(const Color &color, size_t ind) override;
    virtual Color getColor(size_t ind) const override;

    virtual void setPosition(const vec2i &coord, size_t ind) override;
    virtual void setPosition(const vec2f &coord, size_t ind) override;
    virtual void setPosition(const vec2d &coord, size_t ind) override;
    virtual void setPosition(int    x, int    y, size_t ind) override;
    virtual void setPosition(float  x, float  y, size_t ind) override;
    virtual void setPosition(double x, double y, size_t ind) override;

    virtual void draw(IRenderWindow *window) override;

    static std::unique_ptr<IPixelsArray> create();
};

class Image : public IImage {
public:
    Image() = default;
    virtual ~Image() = default;

    void create(unsigned int width, unsigned int height, const Color &color = Color(0, 0, 0)) override;
    void create(vec2u size,                              const Color &color = Color(0, 0, 0)) override;

    void create(unsigned int width, unsigned int height, const Color *pixels) override;
    void create(vec2u size,                              const Color *pixels) override;

    bool loadFromFile(const std::string &filename) override;

    vec2u getSize() const override;
    void setPixel(unsigned int x, unsigned int y, const Color &color) override;
    void setPixel(vec2u pos,                      const Color &color) override;

    Color getPixel(unsigned int x, unsigned int y) const override;
    Color getPixel(vec2u pos)                      const override;

    virtual void draw(IRenderWindow *window) override;

    static std::unique_ptr<IImage> create();

private:
    sf::Image image_;

    friend class Texture;
};

class Texture : public ITexture
{
public:
    Texture();
    ~Texture() override = default;

    bool create(unsigned int width, unsigned int height)                                     override;
    bool loadFromFile(const std::string& filename,          const IntRect& area = IntRect()) override;
    bool loadFromMemory(const void* data, std::size_t size, const IntRect& area = IntRect()) override;
    vec2u getSize()                                                                    const override;
    std::unique_ptr<IImage> copyToImage()                                              const override;
    void update(const IImage *image)                                                         override;
    void update(const Color *pixels)                                                         override;
    void update(const Color *pixels, unsigned int width, unsigned int height,
                                     unsigned int     x, unsigned int      y) override;

    static std::unique_ptr<ITexture> create();

private:
    sf::Texture texture_;

    friend class Sprite;
};

class Sprite : public ISprite {
private:
    sf::Sprite sprite_;

public:
    virtual ~Sprite() = default;

    void setTexture(const ITexture *texture, bool reset_rect = false) override;
    void setTextureRect(const IntRect &rectangle) override;
    void setPosition(float x, float y) override;
    void setPosition(const vec2f &pos) override;
    void setScale(float factorX, float factorY) override;
    vec2i getSize() const override;
    void setColor(const Color &color) override;
    void setRotation(float angle) override;
    const vec2f getPosition() const override;
    IntRect getGlobalBounds() const override;
    void draw(IRenderWindow *window) override;
};

class Font : public IFont {
private:
    sf::Font font_;

    friend class Text;

public:
    bool loadFromFile(const std::string& filename) override;
};

class Text : public IText {
private:
    sf::Text text_;

    friend class RenderWindow;

public:
    virtual ~Text() = default;

    void draw(IRenderWindow *window) override;
    void setString(const std::string& string) override;
    void setFont(const IFont* font) override;
    void setCharacterSize(unsigned int size) override;
    void setStyle(uint32_t style) override;
    void setFillColor(const Color* color) override;
    void setOutlineColor(const Color* color) override;
    void setOutlineThickness(float thickness) override;
};

} // namespace sfm
} // namespace psapi

#endif // SFM_IMPLEMENTATION_HPP

