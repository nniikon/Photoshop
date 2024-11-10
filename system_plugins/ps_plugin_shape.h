#ifndef PHOTOSHOP_PLUGIN_SHAPE_H
#define PHOTOSHOP_PLUGIN_SHAPE_H

#include "ps_bar.h"
#include "api_canvas.hpp"
#include "api_sfm.hpp"

extern "C" {
    bool loadPlugin();
    void unloadPlugin();
}

template <typename T>
class ShapeAction : public ps::ButtonAction {
public:
    ShapeAction();
    virtual bool operator()(const psapi::IRenderWindow* renderWindow, 
                            const psapi::sfm::Event& event) override;
    virtual bool activate() override;

private:
    void handleMousePressed(const psapi::sfm::Event& event, const psapi::sfm::vec2i& mouse_pos);
    void handleMouseReleased(psapi::ILayer* active_layer);
    void updateTempLayer(psapi::ILayer* layer, const psapi::sfm::vec2i& current_pos);
    void transferFinalShapeToLayer(psapi::ILayer* active_layer);
    void clearLayer(psapi::ILayer* layer);

    std::unique_ptr<T> ellipse_ = nullptr;
    psapi::ICanvas* canvas_ = nullptr;
    bool is_mouse_down_ = false;
    psapi::sfm::vec2i mouse_starting_point_ = {100, 100};
    psapi::sfm::vec2i last_mouse_pos_ = {0, 0};
    bool is_new_frame_ = true;
};

#endif // PHOTOSHOP_PLUGIN_SHAPE_H
