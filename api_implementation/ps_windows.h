#ifndef API_IMPLEMENTATION_PS_WINDOW_H_
#define API_IMPLEMENTATION_PS_WINDOW_H_

#include "api_photoshop.hpp"

namespace ps {

class AWindow : public psapi::IWindow {
public:
    virtual ~AWindow() = default;

    virtual int64_t getId() const override;

    virtual       psapi::IWindow* getWindowById(psapi::wid_t id)       override;
    virtual const psapi::IWindow* getWindowById(psapi::wid_t id) const override;

protected:
    psapi::wid_t id_ = psapi::kInvalidWindowId;
};

} // namespace

class AWindowContainer : public psapi::IWindowContainer {
public:
    virtual int64_t getId() const override;

protected:
    psapi::wid_t id_ = psapi::kInvalidWindowId;
};

class AWindowVector : public psapi::IWindowVector {
public:
    virtual ~AWindowVector() = default;

    virtual int64_t getId() const override;
    virtual       psapi::IWindow* getWindowById(psapi::wid_t id)       override;
    virtual const psapi::IWindow* getWindowById(psapi::wid_t id) const override;

    virtual void addWindow(std::unique_ptr<IWindow> window) override;
    virtual void removeWindow(psapi::wid_t id)              override;

protected:
    void assertDuplicateId(psapi::wid_t id);
    void drawChildren(psapi::ARenderWindow* renderWindow);
    bool updateChildren(const psapi::ARenderWindow* renderWindow,
                        const psapi::sfm::Event& event);


    psapi::wid_t id_ = psapi::kInvalidWindowId;
};

class RootWindow : public AWindowVector {
public:
    virtual void draw  (      psapi::ARenderWindow* renderWindow) override;
    virtual bool update(const psapi::ARenderWindow* renderWindow,
                        const psapi::sfm::Event& event)           override;
    virtual int64_t getId() const override;

    virtual psapi::vec2i getPos()  const          override;
    virtual psapi::vec2i getSize() const          override;
    virtual void setParent(const IWindow* parent) override;
    virtual void forceActivate()                  override;
    virtual void forceDeactivate()                override;

private:
    bool is_active_ = true;
};

#endif  // API_IMPLEMENTATION_PS_WINDOW_H_
