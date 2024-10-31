#ifndef API_IMPLEMENTATION_PS_BAR_H_
#define API_IMPLEMENTATION_PS_BAR_H_

#include "api_bar.hpp"

namespace ps {

class ButtonAction {
public:
    virtual bool operator()(const psapi::IRenderWindow* renderWindow,
                            const psapi::sfm::Event& event) = 0;
    virtual bool activate() = 0;
    virtual ~ButtonAction() = default;
};

// Note: You should implement update() method
class ABarButton : public psapi::IBarButton {
public: 
    ABarButton(std::unique_ptr<psapi::sfm::ISprite> sprite,
               const psapi::IBar* toolbar,
               std::unique_ptr<ButtonAction> action);

    virtual void draw(psapi::IRenderWindow* renderWindow)         override;
    virtual bool update(const psapi::IRenderWindow* renderWindow,
                        const psapi::sfm::Event& event)           override;

    virtual psapi::wid_t getId() const override;

    virtual       psapi::IWindow* getWindowById(psapi::wid_t id)       override;
    virtual const psapi::IWindow* getWindowById(psapi::wid_t id) const override;

    virtual psapi::vec2i getPos()  const override;
    virtual psapi::vec2u getSize() const override;

    virtual void setParent(const IWindow* parent) override;

    virtual void forceDeactivate()                override;
    virtual void forceActivate()                  override;

    virtual void  setState(State state) override;
    virtual State getState()      const override;

    virtual bool isWindowContainer() const override;

    virtual bool isActive() const override;

protected:
    psapi::wid_t id_ = psapi::kInvalidWindowId;

    const psapi::IWindow* parent_ = nullptr;
    const psapi::IBar*    toolbar_ = nullptr;
    bool is_active_ = true;

    psapi::vec2i pos_;
    psapi::vec2i size_;

    std::unique_ptr<psapi::sfm::ISprite> sprite_;

    State state_ = State::Normal;

    std::unique_ptr<ButtonAction> action_;
};

class ABar : public psapi::IBar {
public:
    ABar(std::unique_ptr<psapi::sfm::ISprite> sprite,
         std::unique_ptr<psapi::sfm::ISprite> hover_button_sprite,
         std::unique_ptr<psapi::sfm::ISprite> pressed_button_sprite,
         std::unique_ptr<psapi::sfm::ISprite> active_button_sprite,
         psapi::vec2i toolbar_pos,
         psapi::vec2i toolbar_size,
         psapi::vec2i side_gap,
         psapi::vec2i inbutton_gap,
         psapi::vec2i button_size,
         int n_buttons_in_row,
         psapi::wid_t id);

    //ABar(const std::string& bar_info_path);
    virtual ~ABar();

    virtual void draw(psapi::IRenderWindow* renderWindow)         override;
    virtual bool update(const psapi::IRenderWindow* renderWindow,
                        const psapi::sfm::Event& event)           override;

    virtual       psapi::IWindow* getWindowById(psapi::wid_t id)       override;
    virtual const psapi::IWindow* getWindowById(psapi::wid_t id) const override;

    virtual psapi::vec2i getPos()  const override;
    virtual psapi::vec2u getSize() const override;
    virtual psapi::wid_t getId()   const override;

    virtual void setParent(const IWindow* parent) override;
    virtual void forceDeactivate()                override;
    virtual void forceActivate()                  override; 

    virtual void addWindow(std::unique_ptr<IWindow> window) override;
    virtual void removeWindow(psapi::wid_t id)              override;

    virtual psapi::ChildInfo getNextChildInfo() const override;

    virtual void finishButtonDraw(psapi::IRenderWindow* renderWindow,
                                  const psapi::IBarButton* button) const override;

    virtual bool isActive() const override;

protected:
    psapi::wid_t id_ = psapi::kInvalidWindowId;

    const psapi::IWindow* parent_ = nullptr;
    bool is_active_ = true;

    psapi::vec2i pos_;
    psapi::vec2i size_;

    psapi::vec2i side_gap_;
    psapi::vec2i inbutton_gap_;
    psapi::vec2i button_size_;

    int n_buttons_in_row_;

    int n_buttons_    = 0;

    mutable int cur_button_it = 0;

    std::unique_ptr<psapi::sfm::ISprite> sprite_;
    std::unique_ptr<psapi::sfm::ISprite> hover_button_sprite_;
    std::unique_ptr<psapi::sfm::ISprite> pressed_button_sprite_;
    std::unique_ptr<psapi::sfm::ISprite> active_button_sprite_;

    void drawChildren(psapi::IRenderWindow* renderWindow);
    void drawMyself  (psapi::IRenderWindow* renderWindow);
    bool updateChildren(const psapi::IRenderWindow* renderWindow,
                        const psapi::sfm::Event& event);

    std::vector<std::unique_ptr<psapi::IBarButton>> windows_;

    ssize_t last_active_button_it = -1;
    void handleDoubleActiveButton();
};

}

#endif // API_IMPLEMENTATION_PS_BAR_H_
