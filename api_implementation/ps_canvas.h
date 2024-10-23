#ifndef API_IMPLEMENTATION_PS_CANVAS_H_
#define API_IMPLEMENTATION_PS_CANVAS_H_

#include "api_canvas.hpp"
#include "api_sfm.hpp"
#include "sfm_implementation.h"

namespace ps {

using psapi::sfm::Color;
using psapi::sfm::vec2i;
using psapi::sfm::vec2f;

class Layer : public psapi::ILayer {
public:
    Layer(vec2i size);
    virtual Color getPixel(vec2i pos)        const override;
    virtual void  setPixel(vec2i pos, Color pixel) override;

    void changeSize(vec2i new_size);

private:
    friend class Canvas;

    static constexpr int kMaxHeight = 1920;
    static constexpr int kMaxWidth  = 1080;

    void assert_pos (vec2i pos) const;
    void assert_size(vec2i size) const;

    vec2i size_;
    std::vector<Color> pixels_;
};

class Canvas : public psapi::ICanvas {
public:
    Canvas(vec2i size);
    virtual ~Canvas() = default;

    virtual void draw(psapi::ARenderWindow* renderWindow)         override;
    virtual bool update(const psapi::ARenderWindow* renderWindow,
                        const psapi::sfm::Event& event)           override;

    virtual       psapi::ILayer* getLayer(size_t index)       override;
    virtual const psapi::ILayer* getLayer(size_t index) const override;

    virtual       psapi::ILayer* getTempLayer()       override;
    virtual const psapi::ILayer* getTempLayer() const override; 

    virtual void cleanTempLayer() override;
    virtual size_t getNLayers() const override;

    virtual bool removeLayer     (size_t index) override;
    virtual bool insertEmptyLayer(size_t index) override;
    virtual bool insertLayer(size_t index, std::unique_ptr<psapi::ILayer> layer) override;

    virtual size_t getActiveLayerIndex()       const override;
    virtual void   setActiveLayerIndex(size_t index) override;

    virtual void setPos  (vec2i pos)   override;
    virtual void setSize (vec2i size)  override;
    virtual void setScale(vec2f scale) override;

private:
    size_t active_layer_ = 0;

    std::unique_ptr<Layer> temp_layer_;
    std::vector<std::unique_ptr<Layer>> layers_;

    void DrawLayer(const Layer& layer, psapi::ARenderWindow* renderWindow);

    vec2i pos_;
    vec2i size_;
    vec2f scale_;
    psapi::sfm::Texture texture_;
    psapi::sfm::Sprite sprite_;
    vec2i last_mouse_pos_;
    bool is_pressed_ = false;
};

} // namespace

#endif  // API_IMPLEMENTATION_PS_CANVAS_H_
