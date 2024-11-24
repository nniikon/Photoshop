#include "api_photoshop.hpp"
#include "api_sfm.hpp"

extern "C" {
    bool   loadPlugin();
    void unloadPlugin();
}

namespace ps {

class SecondaryPanel;

using psapi::sfm::Color;

const psapi::wid_t kSecondaryPanelWindowId        = 6661;
const psapi::wid_t kSecondaryPanelColorPalette    = 6662;
const psapi::wid_t kSecondaryPanelThicknessSlider = 6663;

class SecondaryPanelButton : public psapi::IWindow {
public:
    SecondaryPanelButton(psapi::wid_t id, 
                         const SecondaryPanel* parent_panel);

    psapi::wid_t getId() const                                 override;
    const psapi::IWindow* getWindowById(psapi::wid_t id) const override;
    psapi::IWindow*       getWindowById(psapi::wid_t id)       override;
    psapi::vec2i getPos() const                                override;
    psapi::vec2u getSize() const                               override;
    void setParent(const IWindow* parent)                      override;
    void forceActivate()                                       override;
    void forceDeactivate()                                     override;
    bool isActive() const                                      override;
    bool isWindowContainer() const                             override;

protected:
    const SecondaryPanel* parent_panel_ = nullptr; 

private:
    psapi::wid_t id_ = psapi::kInvalidWindowId;
};

class ColorPalette : public SecondaryPanelButton {
public:
    ColorPalette(psapi::wid_t id, 
                 const SecondaryPanel* parent_panel);
    ~ColorPalette() = default;

    void draw(psapi::IRenderWindow* render_window) override;
    bool update(const psapi::IRenderWindow* render_window,
                const psapi::Event& event) override;

    void  enable();
    void disable();

    Color getColor() const;
    void  setColor(Color color);

private:
    Color color_ = {0, 0, 0, 255};
};

class SecondaryPanel : public psapi::IWindowVector {
public:
    SecondaryPanel();
    ~SecondaryPanel() = default;

    void draw(psapi::IRenderWindow* renderWindow)              override;
    bool update(const psapi::IRenderWindow* renderWindow, 
                const psapi::Event& event)                     override;
    psapi::wid_t getId() const                                 override;
    const psapi::IWindow* getWindowById(psapi::wid_t id) const override;
    psapi::IWindow*       getWindowById(psapi::wid_t id)       override;
    psapi::vec2i getPos() const                                override;
    psapi::vec2u getSize() const                               override;
    void setParent(const IWindow* parent)                      override;
    void forceActivate()                                       override;
    void forceDeactivate()                                     override;
    bool isActive() const                                      override;

    void  enable();
    void disable();

    Color getColor() const;
    void  setColor(Color color);

    int  getThickness() const;
    void setThickness(int thickness);

private:
    std::unique_ptr<psapi::sfm::ITexture> texture_ = nullptr;
    std::unique_ptr<psapi::sfm::ISprite>  sprite_  = nullptr;

    ColorPalette color_palette_;

    bool is_enabled_ = false;
};

} // namespace
