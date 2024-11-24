#include "ps_plugin_canvas.h"

#include "loguru.hpp"

#include <memory>

bool loadPlugin() {
    std::unique_ptr<psapi::ICanvas> canvas = std::make_unique<ps::ScalableCanvas>(
            psapi::vec2i{600, 950},
            psapi::vec2i{300, 50}
    );

    psapi::getRootWindow()->addWindow(std::move(canvas));
    return true;
}

using namespace ps;

ScalableCanvas::ScalableCanvas(const psapi::vec2i size, const psapi::vec2i position)
    : Canvas(size, position) {
}

const int kLeftBarWidth  = 10;
const int kLeftBarHeight = 10;
const int kLeftBarOffset = 3;
const psapi::sfm::Color kLeftBarColor = {50, 50, 50, 255};
const int kSliderGap = 5;

void ScalableCanvas::draw(psapi::IRenderWindow* renderWindow) {
    Canvas::draw(renderWindow);

    drawLeftBar(renderWindow);
}

psapi::vec2i ScalableCanvas::getLeftBarPosTop()  const {
    return {getPos().x - kLeftBarOffset - kLeftBarWidth,
            getPos().y};
}

psapi::vec2i ScalableCanvas::getLeftBarPosDown() const {
    return {getPos().x - kLeftBarOffset - kLeftBarWidth,
            getPos().y + (int)getSize().y - kLeftBarHeight};
}

psapi::vec2i ScalableCanvas::getLeftSlidePos()   const {
    psapi::vec2i left_bar_pos_top  = getLeftBarPosTop();
    psapi::vec2i left_bar_pos_down = getLeftBarPosDown();

    int heighest_slide_pos = left_bar_pos_top.y  + kLeftBarHeight + kSliderGap; 
    int lowest_slide_pos   = left_bar_pos_down.y - kSliderGap - (int)getLeftSlideHeight();

    if (Canvas::getOffset().y <= 0.001f) {
        return {left_bar_pos_top.x, heighest_slide_pos};
    }

    float slideness_coef = Canvas::getOffset().y / Canvas::getMaxOffset().y;
    int current_slide_pos = (int)((float)(lowest_slide_pos - heighest_slide_pos) * slideness_coef + (float)heighest_slide_pos);
    return {left_bar_pos_top.x, current_slide_pos};
}

unsigned int ScalableCanvas::getLeftSlideHeight() const {
    psapi::vec2i left_bar_pos_top  = getLeftBarPosTop();
    psapi::vec2i left_bar_pos_down = getLeftBarPosDown();

    int slide_max_height = left_bar_pos_down.y - left_bar_pos_top.y - kLeftBarHeight - 2 * kSliderGap;

    return (unsigned int)((float)slide_max_height / Canvas::getScale().y);
}

void ScalableCanvas::drawLeftBar(psapi::IRenderWindow* renderWindow) const {
    psapi::vec2u left_bar_size = {kLeftBarWidth, kLeftBarHeight};

    psapi::vec2i left_bar_pos_top  = getLeftBarPosTop();
    psapi::vec2i left_bar_pos_down = getLeftBarPosDown();

    auto left_bar_top  = psapi::sfm::IRectangleShape::create(left_bar_size);
    auto left_bar_down = psapi::sfm::IRectangleShape::create(left_bar_size);

    left_bar_top ->setFillColor(kLeftBarColor);
    left_bar_down->setFillColor(kLeftBarColor);

    left_bar_top ->setPosition(left_bar_pos_top);
    left_bar_down->setPosition(left_bar_pos_down);

    left_bar_top ->draw(renderWindow);
    left_bar_down->draw(renderWindow);

    psapi::vec2u slider_size = {kLeftBarWidth, getLeftSlideHeight()};

    auto slider = psapi::sfm::IRectangleShape::create(psapi::vec2u{kLeftBarWidth, getLeftSlideHeight()});
    slider->setPosition(getLeftSlidePos());
    slider->setFillColor(kLeftBarColor);

    slider->draw(renderWindow);
}

bool ScalableCanvas::update(const psapi::IRenderWindow* render_window,
                            const psapi::Event& event) {
    return Canvas::update(render_window, event) ||
           handleLeftBar(render_window, event);
}

static inline bool isMouseInRect(psapi::vec2i pos, psapi::vec2i size,
                                 psapi::vec2i mouse_pos) {
    return (pos.x <= mouse_pos.x && mouse_pos.x < pos.x + size.x &&
            pos.y <= mouse_pos.y && mouse_pos.y < pos.y + size.y);
}

bool ScalableCanvas::handleLeftBar(const psapi::IRenderWindow* render_window,
                                   const psapi::Event& event) {

    vec2i mouse_pos = psapi::sfm::Mouse::getPosition(render_window);

    vec2i size       = {kLeftBarWidth, kLeftBarHeight};
    vec2i slide_size = {kLeftBarWidth, (int)getLeftSlideHeight()};

    if (event.type == psapi::sfm::Event::MouseButtonPressed) {
        if      (isMouseInRect(getLeftBarPosTop(),        size, mouse_pos)) { is_left_bar_top_pressed_  = true; } 
        else if (isMouseInRect(getLeftBarPosDown(),       size, mouse_pos)) { is_left_bar_down_pressed_ = true; }
        else if (isMouseInRect(getLeftSlidePos(),   slide_size, mouse_pos)) { is_left_slide_pressed_    = true;
                                                                              last_mouse_pos_ = psapi::vec2i{event.mouseButton.x,
                                                                                                             event.mouseButton.y}; }
    }
    else if (event.type == psapi::sfm::Event::MouseButtonReleased) {
        is_left_bar_top_pressed_  = false;
        is_left_bar_down_pressed_ = false;
        is_left_slide_pressed_    = false;
    }

    if (is_left_bar_top_pressed_) {
        LOG_F(INFO, "left bar top pressed\n");
        Canvas::setOffset(getOffset() - psapi::vec2f{0.f, 0.005f});
    }

    if (is_left_bar_down_pressed_) {
        LOG_F(INFO, "left bar down pressed\n");
        Canvas::setOffset(getOffset() + psapi::vec2f{0.f, 0.005f});
    }

    if (is_left_slide_pressed_ && event.type == psapi::sfm::Event::MouseMoved) {
        float offset_y = (float)(event.mouseMove.y - last_mouse_pos_.y) / float(getLeftBarPosDown().y - getLeftBarPosTop().y);
        Canvas::setOffset(getOffset() + psapi::vec2f{0.0f, offset_y});
        last_mouse_pos_ = psapi::vec2i{event.mouseMove.x, event.mouseMove.y};
    }

    if (event.type == psapi::sfm::Event::MouseWheelScrolled) {
        float offset_y = -10.f * event.mouseWheel.delta / float(getLeftBarPosDown().y - getLeftBarPosTop().y);
        Canvas::setOffset(getOffset() + psapi::vec2f{0.0f, offset_y});
    }

    return true;
}
