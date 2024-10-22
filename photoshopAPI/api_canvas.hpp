#ifndef API_CANVAS_HPP
#define API_CANVAS_HPP

#include "api_photoshop.hpp"

#include <cstdint>
#include <memory>

namespace psapi {

const wid_t kCanvasWindowId = 100;

class ILayer {
public:
    virtual uint32_t getPixel(vec2i pos) const = 0;
    virtual void     setPixel(vec2i pos, uint32_t pixel) = 0;
};

class ICanvas : public IWindow {
public:
    virtual ILayer*       getLayer(size_t index)       = 0;
    virtual const ILayer* getLayer(size_t index) const = 0;

    virtual ILayer*       getTempLayer()       = 0;
    virtual const ILayer* getTempLayer() const = 0;

    virtual void cleanTempLayer() = 0;

    virtual size_t getNLayers() const = 0;

    virtual bool removeLayer(size_t index) = 0;
    virtual bool insertLayer(size_t index, std::unique_ptr<ILayer> layer) = 0;

    virtual vec2i getMousePosition() const = 0;
    virtual bool isPressed()         const = 0;
};

} // namespace

#endif // API_CANVAS_HPP
