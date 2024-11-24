#ifndef PHOTOSHOP_PLUGIN_CANVAS_H
#define PHOTOSHOP_PLUGIN_CANVAS_H

#include "ps_canvas.h"

extern "C" {
    bool   loadPlugin();
    void unloadPlugin();
}

namespace ps {

template <typename T>
class ScrollableDecorator : public T {
    static_assert(std::is_base_of<Scrollable, T>::value, 
                  "T must derive from Scrollable");

public:
    template <typename... Args>
    explicit ScrollableDecorator(Args&&... args);

    virtual void draw(psapi::sfm::IRenderWindow* render_window) override;
    virtual bool update(const psapi::sfm::IRenderWindow* render_window,
                        const psapi::sfm::Event& event) override;

private:
    void drawScrollBar(psapi::IRenderWindow* render_window) const;
    bool handleScrollBar(const psapi::IRenderWindow* render_window, 
                         const psapi::Event& event);

    psapi::vec2i getScrollBarPosition() const;
    psapi::vec2i getScrollBarSize()     const;
    psapi::vec2i getSliderPosition()    const;
    unsigned int getSliderHeight()      const;

    bool is_slider_dragged_ = false;
    psapi::vec2i last_mouse_pos_ = {0, 0};
};

template <typename T>
template <typename... Args>
ScrollableDecorator<T>::ScrollableDecorator(Args&&... args)
    : T(std::forward<Args>(args)...) {
}

template <typename T>
void ScrollableDecorator<T>::draw(psapi::sfm::IRenderWindow* render_window) {
    T::draw(render_window);
    drawScrollBar(render_window);
}

template <typename T>
bool ScrollableDecorator<T>::update(const psapi::sfm::IRenderWindow* render_window, 
                                    const psapi::sfm::Event& event) {
    return T::update(render_window, event) || handleScrollBar(render_window, event);
}

template <typename T>
void ScrollableDecorator<T>::drawScrollBar(psapi::IRenderWindow* render_window) const {
    psapi::vec2i bar_pos = getScrollBarPosition();
    psapi::vec2i bar_size = getScrollBarSize();
    auto scrollbar = psapi::sfm::IRectangleShape::create(static_cast<unsigned int>(bar_size.x), 
                                                         static_cast<unsigned int>(bar_size.y));
    scrollbar->setPosition(bar_pos);
    scrollbar->setFillColor(psapi::sfm::Color(50, 50, 50, 255));
    scrollbar->draw(render_window);

    psapi::vec2i slider_pos = getSliderPosition();
    psapi::vec2i slider_size = {bar_size.x, static_cast<int>(getSliderHeight())};
    auto slider = psapi::sfm::IRectangleShape::create(static_cast<unsigned int>(slider_size.x), 
                                                      static_cast<unsigned int>(slider_size.y));
    slider->setPosition(slider_pos);
    slider->setFillColor(psapi::sfm::Color(100, 100, 100, 255));
    slider->draw(render_window);
}

static inline bool isMouseInRect(psapi::vec2i pos, psapi::vec2i size,
                                 psapi::vec2i mouse_pos) {
    return (pos.x <= mouse_pos.x && mouse_pos.x < pos.x + size.x &&
            pos.y <= mouse_pos.y && mouse_pos.y < pos.y + size.y);
}

template <typename T>
bool ScrollableDecorator<T>::handleScrollBar(const psapi::IRenderWindow* render_window, 
                                             const psapi::Event& event) {
    psapi::vec2i mouse_pos = psapi::sfm::Mouse::getPosition(render_window);

    psapi::vec2i slider_pos = getSliderPosition();
    psapi::vec2i slider_size = {getScrollBarSize().x, static_cast<int>(getSliderHeight())};

    if (event.type == psapi::sfm::Event::MouseButtonPressed) {
        if (isMouseInRect(slider_pos, slider_size, mouse_pos)) {
            is_slider_dragged_ = true;
            last_mouse_pos_ = mouse_pos;
        }
    } else if (event.type == psapi::sfm::Event::MouseButtonReleased) {
        is_slider_dragged_ = false;
    }

    if (is_slider_dragged_ && event.type == psapi::sfm::Event::MouseMoved) {
        float delta_y = static_cast<float>(mouse_pos.y - last_mouse_pos_.y);
        float bar_height = static_cast<float>(getScrollBarSize().y);
        float offset_change = delta_y / bar_height * T::getMaxOffset().y;
        T::setOffset(T::getOffset() + psapi::vec2f{0.0f, offset_change});
        last_mouse_pos_ = mouse_pos;
    }

    if (event.type == psapi::sfm::Event::MouseWheelScrolled) {
        float delta_y = -10.f * event.mouseWheel.delta;
        T::setOffset(T::getOffset() + psapi::vec2f{0.0f, delta_y});
    }

    return true;
}

template <typename T>
psapi::vec2i ScrollableDecorator<T>::getScrollBarPosition() const {
    return {T::getPos().x + static_cast<int>(T::getSize().x) + 5, T::getPos().y};
}

template <typename T>
psapi::vec2i ScrollableDecorator<T>::getScrollBarSize() const {
    return {10, static_cast<int>(T::getSize().y)};
}

template <typename T>
psapi::vec2i ScrollableDecorator<T>::getSliderPosition() const {
    float offset_coef = T::getOffset().y / T::getMaxOffset().y;
    int slider_y = static_cast<int>(offset_coef * (getScrollBarSize().y - getSliderHeight()));
    return {getScrollBarPosition().x, getScrollBarPosition().y + slider_y};
}

template <typename T>
unsigned int ScrollableDecorator<T>::getSliderHeight() const {
    float scale_ratio = T::getSize().y / (T::getSize().y + T::getMaxOffset().y);
    return static_cast<unsigned int>(getScrollBarSize().y * scale_ratio);
}

class ScalableCanvas : public ps::Canvas {
public:
    ScalableCanvas(psapi::vec2i size, psapi::vec2i pos);

    virtual void draw(psapi::sfm::IRenderWindow* render_window)         override;
    virtual bool update(const psapi::sfm::IRenderWindow* render_window,
                        const psapi::sfm::Event& event)                 override;
private:
    void drawLeftBar(psapi::IRenderWindow* renderWindow) const;
    bool handleLeftBar(const psapi::IRenderWindow* render_window,
                       const psapi::Event& event);

    psapi::vec2i getLeftBarPosTop()   const;
    psapi::vec2i getLeftBarPosDown()  const;
    psapi::vec2i getLeftSlidePos()    const;
    unsigned int getLeftSlideHeight() const;

    bool is_left_bar_top_pressed_  = false;
    bool is_left_bar_down_pressed_ = false; 

    bool is_left_slide_pressed_    = false;
    psapi::vec2i last_mouse_pos_ = {0.f, 0.f};
};

} // namespace

#endif // PHOTOSHOP_PLUGIN_CANVAS_H
