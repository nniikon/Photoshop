#ifndef PHOTOSHOP_PLUGIN_CLEAR_H
#define PHOTOSHOP_PLUGIN_CLEAR_H

#include "ps_bar.h"
#include "api_canvas.hpp"

extern "C" {
    bool onLoadPlugin();
    void onUnloadPlugin();
}

class ClearAction : public ps::ButtonAction {
public:
    ClearAction();
    virtual bool operator()(const psapi::IRenderWindow* renderWindow,
                            const psapi::sfm::Event& event) override;
    virtual bool activate() override;

private:
    psapi::ICanvas* canvas_ = nullptr;

    void clearCanvas();
};

#endif // PHOTOSHOP_PLUGIN_CLEAR_H
