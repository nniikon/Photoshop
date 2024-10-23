#ifndef RENDER_WINDOW_API_HPP
#define RENDER_WINDOW_API_HPP

#include <cstdlib>
#include <memory>

namespace psapi
{

namespace sfm
{

class Event;
class ARenderWindow;

template<typename T>
struct Vec2D
{
    T x;
    T y;
};

using vec2i = Vec2D<int>;
using vec2u = Vec2D<unsigned int>;
using vec2f = Vec2D<float>;
using vec2d = Vec2D<double>;

struct Color
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    Color()                              = default;
    Color &operator=(const Color &color) = default;
    Color(const Color &color)            = default;

    Color(uint8_t init_r, uint8_t init_g, uint8_t init_b, uint8_t init_a);
    ~Color() = default;


    Color &operator+=(const Color &color);
    Color &operator*=(const Color &color);
    Color &operator*=(const double cf);
    Color &operator*=(const float cf);
};

Color operator+(const Color &x, const Color &y);
Color operator*(const Color &x, const Color &y);
Color operator*(const Color &x, const float cf);
Color operator*(const Color &x, const double cf);
Color operator*(const float cf, const Color &x);
Color operator*(const double cf, const Color &x);

class Drawable
{
public:
    virtual ~Drawable() = default;

    virtual void draw(ARenderWindow *window) = 0;
};

class AImage : public Drawable
{
public:
    virtual ~AImage() = default;

    virtual void setColor(const Color &color, size_t ind) = 0;

    virtual void setPosition(const vec2i &coord, size_t ind) = 0;
    virtual void setPosition(const vec2f &coord, size_t ind) = 0;
    virtual void setPosition(const vec2d &coord, size_t ind) = 0;
    virtual void setPosition(int x, int y, size_t ind) = 0;
    virtual void setPosition(float x, float y, size_t ind) = 0;
    virtual void setPosition(double x, double y, size_t ind) = 0;
};

struct IntRect
{
    int top_x;
    int top_y;
    int width;
    int height;
};

class ATexture
{
public:
    virtual ~ATexture() = default;

    virtual bool create(unsigned int width, unsigned int height)                                     = 0;
    virtual bool loadFromFile  (const std::string& filename,        const IntRect& area = IntRect()) = 0;
    virtual bool loadFromMemory(const void* data, std::size_t size, const IntRect& area = IntRect()) = 0;
    virtual bool loadFromImage (const AImage *image,                const IntRect& area = IntRect()) = 0;
    virtual vec2u getSize() const                                                                    = 0;
    virtual std::unique_ptr<AImage> copyToImage() const                                              = 0;
    virtual void update(const Color *pixels)                                                         = 0;
    virtual void update(const Color *pixels, unsigned int width, unsigned int height,
                                             unsigned int x,     unsigned int y) = 0;
    virtual ATexture& operator =(const ATexture& right) = 0;
};

class ASprite : public Drawable
{
public:
    virtual ~ASprite() = default;

    virtual void setTexture(const ATexture *texture, bool reset_rect = false) = 0;
    virtual void setTextureRect(const IntRect &rectangle) = 0;

    virtual void setPosition(float x, float y) = 0;
    virtual void setPosition(const vec2f &pos) = 0;

    virtual void setScale(float factorX, float factorY) = 0;

    virtual void setColor(const Color &color) = 0;

    virtual void setRotation(float angle) = 0;

    virtual const vec2f getPosition() const = 0;
    virtual IntRect getGlobalBounds() const = 0;

    virtual void draw(ARenderWindow *window) = 0;
};

class AFont
{
public:
    virtual ~AFont() = default;

    virtual bool loadFromFile(const std::string& filename) = 0;
};

class AText : public Drawable
{
public:
    enum Style
    {
        Regular       = 0,
        Bold          = 1 << 0,
        Italic        = 1 << 1,
        Underlined    = 1 << 2,
        StrikeThrough = 1 << 3
    };

    virtual ~AText() = default;

    virtual void draw(ARenderWindow *window)          = 0;
    virtual void setString(const std::string& string) = 0;
    virtual void setFont(const AFont* font)           = 0;
    virtual void setCharacterSize(unsigned int size)  = 0;
    virtual void setStyle(uint32_t style)             = 0;
    virtual void setFillColor(const Color* color)     = 0;
    virtual void setOutlineColor(const Color* color)  = 0;
    virtual void setOutlineThickness(float thickness) = 0;
};

class ARenderWindow
{
public:
    virtual ~ARenderWindow() = default;

    virtual bool isOpen()  = 0;
    virtual void clear()   = 0;
    virtual void display() = 0;
    virtual void close()   = 0;

    virtual bool pollEvent(Event& event) = 0;

    virtual void draw(Drawable *target) = 0;
};

class Mouse
{
public:
    enum class Button
    {
        Left,
        Right,
        Middle,

        XButton1,
        XButton2,
    };

    enum class Wheel
    {
        Vertical,
        Horizontal,
    };

    Mouse() = delete;

    static bool isButtonPressed(Button button);

    static vec2i getPosition();
    static vec2i getPosition(const ARenderWindow *relative_to);

    static void setPosition(const vec2i &position);
    static void setPosition(const vec2i &position, const ARenderWindow *relative_to);
};

class Keyboard
{
public:
    enum class Key
    {
        Unknown = -1, ///< Unhandled key
        A = 0,        ///< The A key
        B,            ///< The B key
        C,            ///< The C key
        D,            ///< The D key
        E,            ///< The E key
        F,            ///< The F key
        G,            ///< The G key
        H,            ///< The H key
        I,            ///< The I key
        J,            ///< The J key
        K,            ///< The K key
        L,            ///< The L key
        M,            ///< The M key
        N,            ///< The N key
        O,            ///< The O key
        P,            ///< The P key
        Q,            ///< The Q key
        R,            ///< The R key
        S,            ///< The S key
        T,            ///< The T key
        U,            ///< The U key
        V,            ///< The V key
        W,            ///< The W key
        X,            ///< The X key
        Y,            ///< The Y key
        Z,            ///< The Z key
        Num0,         ///< The 0 key
        Num1,         ///< The 1 key
        Num2,         ///< The 2 key
        Num3,         ///< The 3 key
        Num4,         ///< The 4 key
        Num5,         ///< The 5 key
        Num6,         ///< The 6 key
        Num7,         ///< The 7 key
        Num8,         ///< The 8 key
        Num9,         ///< The 9 key
        Escape,       ///< The Escape key
        LControl,     ///< The left Control key
        LShift,       ///< The left Shift key
        LAlt,         ///< The left Alt key
        LSystem,      ///< The left OS specific key: window (Windows and Linux), apple (MacOS X), ...
        RControl,     ///< The right Control key
        RShift,       ///< The right Shift key
        RAlt,         ///< The right Alt key
        RSystem,      ///< The right OS specific key: window (Windows and Linux), apple (MacOS X), ...
        Menu,         ///< The Menu key
        LBracket,     ///< The [ key
        RBracket,     ///< The ] key
        Semicolon,    ///< The ; key
        Comma,        ///< The , key
        Period,       ///< The . key
        Quote,        ///< The ' key
        Slash,        ///< The / key
        Backslash,    ///< The \ key
        Tilde,        ///< The ~ key
        Equal,        ///< The = key
        Hyphen,       ///< The - key (hyphen)
        Space,        ///< The Space key
        Enter,        ///< The Enter/Return keys
        Backspace,    ///< The Backspace key
        Tab,          ///< The Tabulation key
        PageUp,       ///< The Page up key
        PageDown,     ///< The Page down key
        End,          ///< The End key
        Home,         ///< The Home key
        Insert,       ///< The Insert key
        Delete,       ///< The Delete key
        Add,          ///< The + key
        Subtract,     ///< The - key (minus, usually from numpad)
        Multiply,     ///< The * key
        Divide,       ///< The / key
        Left,         ///< Left arrow
        Right,        ///< Right arrow
        Up,           ///< Up arrow
        Down,         ///< Down arrow
        Numpad0,      ///< The numpad 0 key
        Numpad1,      ///< The numpad 1 key
        Numpad2,      ///< The numpad 2 key
        Numpad3,      ///< The numpad 3 key
        Numpad4,      ///< The numpad 4 key
        Numpad5,      ///< The numpad 5 key
        Numpad6,      ///< The numpad 6 key
        Numpad7,      ///< The numpad 7 key
        Numpad8,      ///< The numpad 8 key
        Numpad9,      ///< The numpad 9 key
        F1,           ///< The F1 key
        F2,           ///< The F2 key
        F3,           ///< The F3 key
        F4,           ///< The F4 key
        F5,           ///< The F5 key
        F6,           ///< The F6 key
        F7,           ///< The F7 key
        F8,           ///< The F8 key
        F9,           ///< The F9 key
        F10,          ///< The F10 key
        F11,          ///< The F11 key
        F12,          ///< The F12 key
        F13,          ///< The F13 key
        F14,          ///< The F14 key
        F15,          ///< The F15 key
        Pause,        ///< The Pause key
    };

    Keyboard() = delete;

    static bool isKeyPressed(Key key);
};

class Event
{
public:
    struct SizeEvent
    {
        unsigned int width;
        unsigned int height;
    };

    struct KeyEvent
    {
        Keyboard::Key code;
        bool          alt;
        bool          control;
        bool          shift;
        bool          system;
    };

    struct TextEvent
    {
        uint32_t unicode;
    };

    struct MouseMoveEvent
    {
        int x;
        int y;
    };

    struct MouseButtonEvent
    {
        Mouse::Button button;
        int           x;
        int           y;
    };

    struct MouseWheelScrollEvent
    {
        Mouse::Wheel wheel;
        float        delta;
        int          x;
        int          y;
    };

    enum EventType
    {
        Unknown,
        Closed,
        Resized,
        LostFocus,
        GainedFocus,
        TextEntered,
        KeyPressed,
        KeyReleased,
        MouseWheelScrolled,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseEntered,
        MouseLeft,
    };

    EventType type;

    union
    {
        SizeEvent             size;
        KeyEvent              key;
        TextEvent             text;
        MouseMoveEvent        mouseMove;
        MouseButtonEvent      mouseButton;
        MouseWheelScrollEvent mouseWheel;
    };
};

} // sfm

} // psapi

#endif // RENDER_WINDOW_API_HPP
