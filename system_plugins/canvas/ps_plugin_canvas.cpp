#include "ps_plugin_canvas.h"

#include "loguru.hpp"

#include <memory>

bool loadPlugin() {
    auto canvas = std::make_unique<ps::ScrollableDecorator<ps::Canvas>>(
            psapi::vec2i{600, 950},
            psapi::vec2i{300, 50}
    );

    psapi::getRootWindow()->addWindow(std::move(canvas));
    return true;
}

using namespace ps;

template <typename T>
template <typename... Args>
ScrollableDecorator<T>::ScrollableDecorator(Args&&... args)
    : T(std::forward<Args>(args)...) {
}

const int kBarWidth  = 10;
const int kBarHeight = 10;
const int kBarOffset = 3;
const psapi::sfm::Color kBarColor = {50, 50, 50, 255};
const int kSliderGap = 5;

template <typename T>
void ScrollableDecorator<T>::draw(psapi::IRenderWindow* renderWindow) {
    Canvas::draw(renderWindow);

    drawLeftBar(renderWindow);
    drawDownBar(renderWindow);
}

template <typename T>
psapi::vec2i ScrollableDecorator<T>::getLeftBarPosTop()  const {
    return {T::getPos().x - kBarOffset - kBarWidth,
            T::getPos().y};
}

template <typename T>
psapi::vec2i ScrollableDecorator<T>::getDownBarPosLeft()  const {
    return {T::getPos().x,
            T::getPos().y + (int)T::getSize().y + kBarOffset};
}

template <typename T>
psapi::vec2i ScrollableDecorator<T>::getLeftBarPosDown() const {
    return {T::getPos().x - kBarOffset - kBarWidth,
            T::getPos().y + (int)T::getSize().y - kBarHeight};
}

template <typename T>
psapi::vec2i ScrollableDecorator<T>::getDownBarPosRight() const {
    return {T::getPos().x + (int)T::getSize().x - kBarWidth,
            T::getPos().y + (int)T::getSize().y + kBarOffset};
}

template <typename T>
psapi::vec2i ScrollableDecorator<T>::getLeftSlidePos()   const {
    psapi::vec2i left_bar_pos_top  = getLeftBarPosTop();
    psapi::vec2i left_bar_pos_down = getLeftBarPosDown();

    int heighest_slide_pos = left_bar_pos_top.y  + kBarHeight + kSliderGap; 
    int lowest_slide_pos   = left_bar_pos_down.y - kSliderGap - (int)getLeftSlideHeight();

    if (Canvas::getOffset().y <= 0.001f) {
        return {left_bar_pos_top.x, heighest_slide_pos};
    }

    float slideness_coef = Canvas::getOffset().y / Canvas::getMaxOffset().y;
    int current_slide_pos = (int)((float)(lowest_slide_pos - heighest_slide_pos) * slideness_coef + (float)heighest_slide_pos);
    return {left_bar_pos_top.x, current_slide_pos};
}

template <typename T>
psapi::vec2i ScrollableDecorator<T>::getDownSlidePos()   const {
    psapi::vec2i down_bar_pos_left  = getDownBarPosLeft();
    psapi::vec2i down_bar_pos_right = getDownBarPosRight();

    int leftest_slide_pos  = down_bar_pos_left.x  + kBarHeight + kSliderGap; 
    int rightest_slide_pos = down_bar_pos_right.x - kSliderGap - (int)getDownSlideWidth();

    if (Canvas::getOffset().x <= 0.001f) {
        return {leftest_slide_pos, down_bar_pos_left.y};
    }

    float slideness_coef = Canvas::getOffset().x / Canvas::getMaxOffset().x;
    int current_slide_pos = (int)((float)(rightest_slide_pos - leftest_slide_pos) * slideness_coef + (float)leftest_slide_pos);
    return {current_slide_pos, down_bar_pos_left.y};
}

template <typename T>
unsigned int ScrollableDecorator<T>::getLeftSlideHeight() const {
    psapi::vec2i left_bar_pos_top  = getLeftBarPosTop();
    psapi::vec2i left_bar_pos_down = getLeftBarPosDown();

    int slide_max_height = left_bar_pos_down.y - left_bar_pos_top.y - kBarHeight - 2 * kSliderGap;

    return (unsigned int)((float)slide_max_height / Canvas::getScale().y);
}

template <typename T>
unsigned int ScrollableDecorator<T>::getDownSlideWidth() const {
    psapi::vec2i down_bar_pos_left  = getDownBarPosLeft();
    psapi::vec2i down_bar_pos_right = getDownBarPosRight();

    int slide_max_height = down_bar_pos_right.x - down_bar_pos_left.x - kBarHeight - 2 * kSliderGap;

    return (unsigned int)((float)slide_max_height / Canvas::getScale().x);
}

template <typename T>
void ScrollableDecorator<T>::drawLeftBar(psapi::IRenderWindow* renderWindow) const {
    psapi::vec2u left_bar_size = {kBarWidth, kBarHeight};

    psapi::vec2i left_bar_pos_top  = getLeftBarPosTop();
    psapi::vec2i left_bar_pos_down = getLeftBarPosDown();

    auto left_bar_top  = psapi::sfm::IRectangleShape::create(left_bar_size);
    auto left_bar_down = psapi::sfm::IRectangleShape::create(left_bar_size);

    left_bar_top ->setFillColor(kBarColor);
    left_bar_down->setFillColor(kBarColor);

    left_bar_top ->setPosition(left_bar_pos_top);
    left_bar_down->setPosition(left_bar_pos_down);

    left_bar_top ->draw(renderWindow);
    left_bar_down->draw(renderWindow);

    auto slider = psapi::sfm::IRectangleShape::create(psapi::vec2u{kBarWidth, getLeftSlideHeight()});
    slider->setPosition(getLeftSlidePos());
    slider->setFillColor(kBarColor);

    slider->draw(renderWindow);
}

template <typename T>
void ScrollableDecorator<T>::drawDownBar(psapi::IRenderWindow* renderWindow) const {
    psapi::vec2u down_bar_size = {kBarWidth, kBarHeight};

    psapi::vec2i down_bar_pos_left  = getDownBarPosLeft();
    psapi::vec2i down_bar_pos_right = getDownBarPosRight();

    auto down_bar_left  = psapi::sfm::IRectangleShape::create(down_bar_size);
    auto down_bar_right = psapi::sfm::IRectangleShape::create(down_bar_size);

    down_bar_left ->setFillColor(kBarColor);
    down_bar_right->setFillColor(kBarColor);

    down_bar_left ->setPosition(down_bar_pos_left);
    down_bar_right->setPosition(down_bar_pos_right);

    down_bar_left ->draw(renderWindow);
    down_bar_right->draw(renderWindow);

    auto slider = psapi::sfm::IRectangleShape::create(psapi::vec2u{getDownSlideWidth(), kBarWidth});
    slider->setPosition(getDownSlidePos());
    slider->setFillColor(kBarColor);

    slider->draw(renderWindow);
}

template <typename T>
std::unique_ptr<psapi::IAction> ScrollableDecorator<T>::createAction(const psapi::sfm::IRenderWindow* renderWindow,
                                                                     const psapi::sfm::Event& event) {
    handleLeftBar(renderWindow, event);
    handleDownBar(renderWindow, event);
    return T::createAction(renderWindow, event);
}

static inline bool isMouseInRect(psapi::vec2i pos, psapi::vec2i size,
                                 psapi::vec2i mouse_pos) {
    return (pos.x <= mouse_pos.x && mouse_pos.x < pos.x + size.x &&
            pos.y <= mouse_pos.y && mouse_pos.y < pos.y + size.y);
}

template <typename T>
bool ScrollableDecorator<T>::handleLeftBar(const psapi::IRenderWindow* render_window,
                                           const psapi::Event& event) {
    bool was_pressed = false;
    vec2i mouse_pos = psapi::sfm::Mouse::getPosition(render_window);

    vec2i size       = {kBarWidth, kBarHeight};
    vec2i slide_size = {kBarWidth, (int)getLeftSlideHeight()};

    if (event.type == psapi::sfm::Event::MouseButtonPressed) {
        if (isMouseInRect(getLeftBarPosTop(),        size, mouse_pos)) {
            is_left_bar_top_pressed_  = true;
            was_pressed = true;
            LOG_F(INFO, "left bar top pressed\n");
        }
        else if (isMouseInRect(getLeftBarPosDown(),       size, mouse_pos)) {
            is_left_bar_down_pressed_  = true;
            was_pressed = true;
            LOG_F(INFO, "left bar down pressed\n");
        }
        else if (isMouseInRect(getLeftSlidePos(),   slide_size, mouse_pos)) { 
            is_left_slide_pressed_    = true;
            was_pressed = true;
            last_mouse_pos_ = psapi::vec2i{event.mouseButton.x,
                                           event.mouseButton.y};
            LOG_F(INFO, "left bar slider pressed\n");
        }
    }
    else if (event.type == psapi::sfm::Event::MouseButtonReleased) {
        is_left_bar_top_pressed_  = false;
        is_left_bar_down_pressed_ = false;
        is_left_slide_pressed_    = false;

        LOG_F(INFO, "left bar released\n");
    }

    if (is_left_bar_top_pressed_) {
        Canvas::setOffset(T::getOffset() - psapi::vec2f{0.f, 0.005f});
    }

    if (is_left_bar_down_pressed_) {
        Canvas::setOffset(T::getOffset() + psapi::vec2f{0.f, 0.005f});
    }

    if (is_left_slide_pressed_ && event.type == psapi::sfm::Event::MouseMoved) {
        float offset_y = (float)(event.mouseMove.y - last_mouse_pos_.y) / float(getLeftBarPosDown().y - getLeftBarPosTop().y);
        Canvas::setOffset(T::getOffset() + psapi::vec2f{0.0f, offset_y});
        last_mouse_pos_ = psapi::vec2i{event.mouseMove.x, event.mouseMove.y};
    }

    return was_pressed;
}

template <typename T>
bool ScrollableDecorator<T>::handleDownBar(const psapi::IRenderWindow* render_window,
                                           const psapi::Event& event) {
    bool was_pressed = false;
    vec2i mouse_pos = psapi::sfm::Mouse::getPosition(render_window);

    vec2i size       = {kBarWidth, kBarHeight};
    vec2i slide_size = {(int)getDownSlideWidth(), kBarHeight};

    if (event.type == psapi::sfm::Event::MouseButtonPressed) {
        if (isMouseInRect(getDownBarPosLeft(), size, mouse_pos)) {
            is_down_bar_left_pressed_  = true;
            was_pressed = true;
            LOG_F(INFO, "down bar left pressed\n");
        }
        else if (isMouseInRect(getDownBarPosRight(),      size, mouse_pos)) {
            is_down_bar_right_pressed_  = true;
            was_pressed = true;
            LOG_F(INFO, "down bar right pressed\n");
        }
        else if (isMouseInRect(getDownSlidePos(),   slide_size, mouse_pos)) { 
            is_down_slide_pressed_    = true;
            was_pressed = true;
            last_mouse_pos_ = psapi::vec2i{event.mouseButton.x,
                                           event.mouseButton.y};
            LOG_F(INFO, "down bar slider pressed\n");
        }
    }
    else if (event.type == psapi::sfm::Event::MouseButtonReleased) {
        is_down_bar_left_pressed_  = false;
        is_down_bar_right_pressed_ = false;
        is_down_slide_pressed_     = false;

        LOG_F(INFO, "down bar released\n");
    }

    if (is_down_bar_left_pressed_) {
        Canvas::setOffset(T::getOffset() - psapi::vec2f{0.005f, 0.f});
    }

    if (is_down_bar_right_pressed_) {
        Canvas::setOffset(T::getOffset() + psapi::vec2f{0.005f, 0.f});
    }

    if (is_down_slide_pressed_ && event.type == psapi::sfm::Event::MouseMoved) {
        float offset_x = (float)(event.mouseMove.x - last_mouse_pos_.x) / float(getDownBarPosRight().x - getDownBarPosLeft().x);
        Canvas::setOffset(T::getOffset() + psapi::vec2f{offset_x, 0.0f});
        last_mouse_pos_ = psapi::vec2i{event.mouseMove.x, event.mouseMove.y};
    }

    return was_pressed;
}
