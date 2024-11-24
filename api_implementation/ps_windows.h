#ifndef API_IMPLEMENTATION_PS_WINDOW_H_
#define API_IMPLEMENTATION_PS_WINDOW_H_

#include "api_photoshop.hpp"

namespace ps {

class RootWindow : public psapi::IWindowVector {
public:
    virtual void draw  (      psapi::IRenderWindow* renderWindow) override;
    virtual bool update(const psapi::IRenderWindow* renderWindow,
                        const psapi::sfm::Event& event)           override;
    virtual int64_t getId() const override;

    virtual psapi::vec2i getPos()  const          override;
    virtual psapi::vec2u getSize() const          override;
    virtual void setParent(const IWindow* parent) override;
    virtual void forceActivate()                  override;
    virtual void forceDeactivate()                override;

    virtual bool isActive() const override;

private:
    bool is_active_ = true;
    psapi::vec2u size_ = {0, 0};
};

} // namespace

#endif  // API_IMPLEMENTATION_PS_WINDOW_H_
