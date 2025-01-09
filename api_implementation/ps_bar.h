#ifndef API_IMPLEMENTATION_PS_BAR_H_
#define API_IMPLEMENTATION_PS_BAR_H_

#include "api_bar.hpp"
#include <vector>

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

    virtual void draw(psapi::IRenderWindow* renderWindow) override;
    virtual std::unique_ptr<psapi::IAction> createAction(const psapi::IRenderWindow* render_window,
                                                         const psapi::Event& event) override;

    virtual psapi::wid_t getId() const override;

    virtual       psapi::IWindow* getWindowById(psapi::wid_t id)       override;
    virtual const psapi::IWindow* getWindowById(psapi::wid_t id) const override;

    virtual psapi::vec2i getPos()  const override;
    virtual psapi::vec2u getSize() const override;

    virtual void setSize(const psapi::vec2u& size) override;
    virtual void setPos(const psapi::vec2i& pos)   override;

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
private:
    struct ChildInfo {
        psapi::vec2i pos;
        psapi::vec2u size;
    };
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

    virtual void draw(psapi::IRenderWindow* renderWindow) override;
    virtual std::unique_ptr<psapi::IAction> createAction(const psapi::IRenderWindow* renderWindow,
                                                         const psapi::Event& event) override;

    virtual       psapi::IWindow* getWindowById(psapi::wid_t id)       override;
    virtual const psapi::IWindow* getWindowById(psapi::wid_t id) const override;

    virtual psapi::vec2i getPos()  const override;
    virtual psapi::vec2u getSize() const override;
    virtual psapi::wid_t getId()   const override;

    virtual void setParent(const IWindow* parent) override;
    virtual void forceDeactivate()                override;
    virtual void forceActivate()                  override; 

    virtual void setSize(const psapi::vec2u& size) override;
    virtual void setPos(const psapi::vec2i& pos)   override;

    virtual void addWindow(std::unique_ptr<IWindow> window) override;
    virtual void removeWindow(psapi::wid_t id)              override;

    virtual void finishButtonDraw(psapi::IRenderWindow* renderWindow,
                                  const psapi::IBarButton* button) const override;

    virtual bool isActive() const override;

    virtual bool unPressAllButtons() override;

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

    std::unique_ptr<psapi::sfm::ISprite> sprite_;
    std::unique_ptr<psapi::sfm::ISprite> hover_button_sprite_;
    std::unique_ptr<psapi::sfm::ISprite> pressed_button_sprite_;
    std::unique_ptr<psapi::sfm::ISprite> active_button_sprite_;

    ChildInfo getChildInfo(int child_num) const;

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
