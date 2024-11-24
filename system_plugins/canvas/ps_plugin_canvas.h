#ifndef PHOTOSHOP_PLUGIN_CANVAS_H
#define PHOTOSHOP_PLUGIN_CANVAS_H

#include "ps_canvas.h"

extern "C" {
    bool   loadPlugin();
    void unloadPlugin();
}

namespace ps {

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

    psapi::vec2i getLeftBarPosTop()  const;
    psapi::vec2i getLeftBarPosDown() const;
    psapi::vec2i getLeftSlidePos()   const;
    unsigned int getLeftSlideHeight() const;

    bool is_left_bar_top_pressed_  = false;
    bool is_left_bar_down_pressed_ = false; 

    bool is_left_slide_pressed_    = false;
    psapi::vec2i last_mouse_pos_ = {0.f, 0.f};
};

} // namespace

#endif // PHOTOSHOP_PLUGIN_CANVAS_H
