#ifndef PS_PLUGIN_FILTERS_H
#define PS_PLUGIN_FILTERS_H

#include "ps_bar.h"
#include "api_sfm.hpp"

namespace psapi {
    class ICanvas;
    class ILayer;
}

extern "C" {
    bool   onLoadPlugin();
    void onUnloadPlugin();
}

class FilterNegAction : public ps::ButtonAction {
public:
    FilterNegAction();
    virtual bool operator()(const psapi::IRenderWindow* renderWindow, 
                            const psapi::sfm::Event& event) override;
    virtual bool activate() override;

private:
    psapi::ICanvas* canvas_ = nullptr;
};

class EmbossAction : public ps::ButtonAction {
public:
    EmbossAction();
    virtual bool operator()(const psapi::IRenderWindow* renderWindow, 
                            const psapi::sfm::Event& event) override;
    virtual bool activate() override;

private:
    psapi::ICanvas* canvas_ = nullptr;
};

class BlurAction : public ps::ButtonAction {
public:
    BlurAction();
    virtual bool operator()(const psapi::IRenderWindow* renderWindow, 
                            const psapi::sfm::Event& event) override;
    virtual bool activate() override;

private:
    void convolve(const float* kernel, psapi::vec2i kernel_size);

    psapi::ICanvas* canvas_ = nullptr;
};

#endif  // PS_PLUGIN_FILTERS_H
