#ifndef PHOTOSHOP_PLUGIN_TOOLBAR_H
#define PHOTOSHOP_PLUGIN_TOOLBAR_H

#include "ps_bar.h"

extern "C" {
    bool   loadPlugin();
    void unloadPlugin();
}

namespace ps {

using psapi::sfm::vec2i;
using psapi::sfm::vec2f;

class ToolbarButton : public ABarButton {
public:
    ToolbarButton(vec2i pos, vec2i size, const char* text);
    ~ToolbarButton() = default;
};

class Toolbar : public ABar {
public:
    Toolbar(vec2i pos, vec2i size);
    ~Toolbar() = default;

    void draw(psapi::ARenderWindow* renderWindow) override;
    bool update(const psapi::ARenderWindow* renderWindow,
                const psapi::sfm::Event& event) override;

    void addChild(std::unique_ptr<ToolbarButton> button);

    virtual vec2i getNextChildPos()  const override;
    virtual vec2i getNextChildSize() const override;

    virtual void finishButtonDraw(psapi::ARenderWindow* renderWindow, const psapi::IBarButton* button) override; 

private:
    constexpr static int gap_size = 16;

    int next_child_index_ = 0;
    int num_children_ = 0;

    vec2i child_size_ = {64, 64};
};

} // namespace

#endif // PHOTOSHOP_PLUGIN_TOOLBAR_H
