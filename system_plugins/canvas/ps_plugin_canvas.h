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
public:
    template <typename... Args>
    ScrollableDecorator(Args&&... args);

    virtual void draw(psapi::sfm::IRenderWindow* render_window) override;
    virtual std::unique_ptr<psapi::IAction> createAction(const psapi::sfm::IRenderWindow* renderWindow,
                                                         const psapi::sfm::Event& event) override;
private:
    void drawLeftBar(psapi::IRenderWindow* renderWindow) const;
    void drawDownBar(psapi::IRenderWindow* renderWindow) const;
    bool handleDownBar(const psapi::IRenderWindow* render_window,
                       const psapi::Event& event);
    bool handleLeftBar(const psapi::IRenderWindow* render_window,
                       const psapi::Event& event);

    psapi::vec2i getLeftBarPosTop()  const;
    psapi::vec2i getLeftBarPosDown() const;
    psapi::vec2i getLeftSlidePos()   const;
    unsigned int getLeftSlideHeight() const;

    psapi::vec2i getDownBarPosLeft()  const;
    psapi::vec2i getDownBarPosRight() const;
    psapi::vec2i getDownSlidePos()   const;
    unsigned int getDownSlideWidth() const;

    bool is_left_bar_top_pressed_  = false;
    bool is_left_bar_down_pressed_ = false;
    bool is_left_slide_pressed_    = false;

    bool is_down_bar_left_pressed_  = false;
    bool is_down_bar_right_pressed_ = false;
    bool is_down_slide_pressed_    = false;

    psapi::vec2i last_mouse_pos_ = {0.f, 0.f};
};

} // namespace

#endif // PHOTOSHOP_PLUGIN_CANVAS_H
