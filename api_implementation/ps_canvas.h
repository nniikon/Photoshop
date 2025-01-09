#ifndef API_IMPLEMENTATION_PS_CANVAS_H_
#define API_IMPLEMENTATION_PS_CANVAS_H_

#include "api_canvas.hpp"
#include "api_sfm.hpp"
#include <vector>

namespace ps {

using psapi::sfm::Color;
using psapi::sfm::vec2i;
using psapi::sfm::vec2u;
using psapi::sfm::vec2f;

using psapi::sfm::ITexture;
using psapi::sfm::ISprite;

struct LayerSnapshot : public psapi::ILayerSnapshot {
    LayerSnapshot(const std::vector<std::shared_ptr<psapi::sfm::Drawable>>& drawables, 
                  const std::vector<Color>& pixels);

    std::vector<std::shared_ptr<psapi::sfm::Drawable>> drawables_;
    std::vector<Color> pixels_;
};

class Layer : public psapi::ILayer {
public:
    Layer(vec2u size);
    virtual Color getPixel(vec2i pos)        const override;
    virtual void  setPixel(vec2i pos, Color pixel) override;

    virtual psapi::drawable_id_t
        addDrawable(std::unique_ptr<psapi::sfm::Drawable> object) override;

    virtual void removeDrawable(psapi::drawable_id_t id) override;

    virtual void removeAllDrawables() override;

    virtual psapi::sfm::vec2u getSize() const override;

    void changeSize(vec2u new_size);

    virtual std::unique_ptr<psapi::ILayerSnapshot> save() override;
    virtual void restore(psapi::ILayerSnapshot* snapshot) override;

private:
    friend class Canvas;

    static constexpr int kMaxHeight = 1920;
    static constexpr int kMaxWidth  = 1080;

    void assert_pos (vec2i pos) const;
    void assert_size(vec2i size) const;

    vec2u size_;
    std::vector<Color> pixels_;
    std::vector<std::shared_ptr<psapi::sfm::Drawable>> drawables_;
};

class Scrollable {
public:
    virtual ~Scrollable() = default;

    virtual psapi::vec2f getOffset() const = 0;

    virtual void setOffset(psapi::vec2f offset) = 0;

    virtual psapi::vec2f getMaxOffset() const = 0;

    virtual psapi::vec2i getPos() const = 0;
};

class CanvasSnapshot : public psapi::ICanvasSnapshot {
public:
    CanvasSnapshot(const std::vector<std::shared_ptr<psapi::sfm::Drawable>>& drawables, 
                   const std::vector<psapi::ILayerSnapshot*>& layers, 
                   const psapi::vec2f& offset);
};

class Canvas : public psapi::ICanvas, public ps::Scrollable {
public:
    Canvas(vec2u size, vec2i pos);
    virtual ~Canvas() = default;

    virtual void draw(psapi::IRenderWindow* renderWindow)         override;
    virtual std::unique_ptr<psapi::IAction> createAction(const psapi::IRenderWindow* renderWindow,
                                                         const psapi::Event& event)          override;

    psapi::wid_t getId()                          const override;
    IWindow* getWindowById(psapi::wid_t id)             override;
    const IWindow* getWindowById(psapi::wid_t id) const override;
    vec2i getPos()                                const override;
    vec2u getSize()                               const override;

    void setParent(const IWindow* parent)               override;
    void forceActivate()                                override;
    void forceDeactivate()                              override;
    bool isWindowContainer()                      const override;

    virtual       psapi::ILayer* getLayer(size_t index)       override;
    virtual const psapi::ILayer* getLayer(size_t index) const override;

    virtual       psapi::ILayer* getTempLayer()       override;
    virtual const psapi::ILayer* getTempLayer() const override; 

    virtual void cleanTempLayer()       override;
    virtual size_t getNumLayers() const override;

    virtual size_t getActiveLayerIndex()       const override;
    virtual void   setActiveLayerIndex(size_t index) override;

    virtual bool insertLayer(size_t index, std::unique_ptr<psapi::ILayer> layer) override;
    virtual bool removeLayer     (size_t index) override;
    virtual bool insertEmptyLayer(size_t index) override;

    virtual void setPos  (const vec2i& pos)   override;
    virtual void setSize (const vec2u& size)  override;

    vec2f getScale() const;
    virtual void setZoom(vec2f scale) override;

    void setOffset(vec2f offset) override;
    vec2f getOffset() const      override;
    vec2f getMaxOffset() const   override;

    virtual vec2i getMousePosition() const override;

    virtual bool isPressedRightMouseButton() const override;
    virtual bool isPressedLeftMouseButton()  const override;
    virtual bool isPressedScrollButton()     const override;

    virtual bool isActive() const override;

    virtual psapi::sfm::Color getCanvasBaseColor() const override;

    virtual std::unique_ptr<psapi::ICanvasSnapshot> save() override;
    virtual void restore(psapi::ICanvasSnapshot* snapshot) override;

private:
    size_t active_layer_ = 0;

    std::unique_ptr<Layer> temp_layer_;
    std::vector<std::unique_ptr<Layer>> layers_;

    void DrawLayer(const Layer& layer, psapi::IRenderWindow* renderWindow);
    bool setLastMousePos(const psapi::IRenderWindow* renderWindow);

    vec2i pos_      = {0, 0};
    vec2u size_     = {0, 0};
    vec2f scale_    = {1.0f, 1.0f};
    vec2f offset_   = {0.0f, 0.0f};

    vec2i last_mouse_pos_ = {0, 0};
    bool is_RMB_pressed_ = false;
    bool is_LMB_pressed_ = false;
    bool is_MMB_pressed_ = false;

    std::unique_ptr<ITexture> texture_;
    std::unique_ptr<ISprite>  sprite_;

    bool is_active_ = true;
};

} // namespace

#endif  // API_IMPLEMENTATION_PS_CANVAS_H_
