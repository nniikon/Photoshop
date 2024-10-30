#ifndef PHOTOSHOP_PLUGIN_BRUSH_H
#define PHOTOSHOP_PLUGIN_BRUSH_H

#include "ps_bar.h"
#include "api_canvas.hpp"
#include <deque>

extern "C" {
    bool loadPlugin();
    void unloadPlugin();
}

class BrushAction : public ps::ButtonAction {
public:
    BrushAction();
    virtual bool operator()(const psapi::IRenderWindow* renderWindow,
                            const psapi::sfm::Event& event) override;

private:
    psapi::ICanvas* canvas_ = nullptr;

    struct MousePoint {
        psapi::sfm::vec2f position;
        float time;
    };
    std::deque<MousePoint> mouse_points_;

    psapi::sfm::vec2i last_mouse_position_ = {0, 0};

    void paintAtPosition(const psapi::sfm::vec2i& position);
    void interpolateAndPaint();
    int evalNumStepsNeeded(const MousePoint& p0,
                           const MousePoint& p1,
                           const MousePoint& p2,
                           const MousePoint& p3);

    static psapi::sfm::vec2i interpolateHermite(const MousePoint& p0, const MousePoint& p1, 
                                                const MousePoint& p2, const MousePoint& p3, 
                                                float t);
};

#endif // PHOTOSHOP_PLUGIN_BRUSH_H
