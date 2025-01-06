#ifndef API_IMPLEMENTATION_PS_WINDOW_H_
#define API_IMPLEMENTATION_PS_WINDOW_H_

#include "api_photoshop.hpp"
#include <deque>
#include <vector>

namespace ps {

class RootWindow : public psapi::IRootWindow {
public:
    virtual void draw  (psapi::IRenderWindow* renderWindow) override;
    virtual std::unique_ptr<psapi::IAction> createAction(const psapi::IRenderWindow* renderWindow,
                                                  const psapi::Event& event) override;
    virtual int64_t getId() const override;

    virtual psapi::vec2i getPos()  const          override;
    virtual psapi::vec2u getSize() const          override;
    virtual void setParent(const IWindow* parent) override;
    virtual void forceActivate()                  override;
    virtual void forceDeactivate()                override;

    virtual void setPos (const psapi::vec2i& pos)  override;
    virtual void setSize(const psapi::vec2u& size) override;

    virtual bool isActive() const override;
    virtual bool isWindowContainer() const override;

    virtual const IWindow* getWindowById(psapi::wid_t id) const override;
    virtual       IWindow* getWindowById(psapi::wid_t id)       override;

    virtual void addWindow   (std::unique_ptr<IWindow> window) override;
    virtual void removeWindow(psapi::wid_t id)                 override;

    virtual psapi::layer_id_t getUpperLayerId() const override;
    virtual psapi::layer_id_t increaseLayerId() override;
    virtual psapi::layer_id_t decreaseLayerId() override;

private:
    std::vector<std::unique_ptr<psapi::IWindow>> windows_;

    bool is_active_ = true;
    psapi::vec2u size_ = {0, 0};
};

class ActionController : public psapi::AActionController {
public:
    ActionController(size_t maxActions = 50ul);
    ~ActionController() = default;

    virtual bool execute(std::unique_ptr<psapi::IAction> action) override;
    virtual bool undo   ()                                       override;
    virtual bool redo   ()                                       override;

private:
    template <typename T>
    void enforceStackLimit(std::deque<T>& stack);

    std::deque<std::unique_ptr<psapi::IAction        >> executed_actions_;
    std::deque<std::unique_ptr<psapi::IUndoableAction>> undone_actions_;

    size_t maxActions_ = 50ul;
};

} // namespace

#endif  // API_IMPLEMENTATION_PS_WINDOW_H_
