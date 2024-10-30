#include "ps_plugin_brush.h"
#include "loguru.hpp"
#include "sfm_implementation.h"
#include <cmath>

constexpr psapi::sfm::IntRect kBrushButtonTextureArea = {0, 0, 64, 64};

constexpr int kBrushRadius = 8;
static psapi::sfm::ITexture* texture = nullptr;

BrushAction::BrushAction() {
    canvas_ = static_cast<psapi::ICanvas*>(psapi::getRootWindow()->getWindowById(psapi::kCanvasWindowId));
}

void BrushAction::paintAtPosition(const psapi::sfm::vec2i& position) {
    auto layer = canvas_->getLayer(canvas_->getActiveLayerIndex());

    int radiusSquared = kBrushRadius * kBrushRadius;

    for (int x = -kBrushRadius; x <= kBrushRadius; ++x) {
        for (int y = -kBrushRadius; y <= kBrushRadius; ++y) {
            if (x * x + y * y <= radiusSquared) {
                psapi::sfm::vec2i pixel_pos = {position.x + x, position.y + y};

                if (pixel_pos.x >= 0 && pixel_pos.x < canvas_->getSize().x &&
                    pixel_pos.y >= 0 && pixel_pos.y < canvas_->getSize().y) {
                    layer->setPixel(pixel_pos, psapi::sfm::Color{255, 0, 0, 255});
                }
            }
        }
    }
}


// Hermite spline interpolation 
//      https://en.wikipedia.org/wiki/Cubic_Hermite_spline#Catmull%E2%80%93Rom_spline
psapi::sfm::vec2i BrushAction::interpolateHermite(const MousePoint& p0, const MousePoint& p1,
                                                  const MousePoint& p2, const MousePoint& p3,
                                                  float t) {
    float t2 = t * t;
    float t3 = t2 * t;

    float h00 = 2 * t3 - 3 * t2 + 1;
    float h10 = t3 - 2 * t2 + t;
    float h01 = -2 * t3 + 3 * t2;
    float h11 = t3 - t2;

    float x = h00 * p1.position.x + h10 * (p2.position.x - p0.position.x) +
              h01 * p2.position.x + h11 * (p3.position.x - p1.position.x);

    float y = h00 * p1.position.y + h10 * (p2.position.y - p0.position.y) +
              h01 * p2.position.y + h11 * (p3.position.y - p1.position.y);

    return {static_cast<int>(std::round(x)), static_cast<int>(std::round(y))};
}

int BrushAction::evalNumStepsNeeded(const MousePoint& p0,
                                    const MousePoint& p1,
                                    const MousePoint& p2,
                                    const MousePoint& p3) {
    float dist2 = (p2.position.x - p1.position.x) * (p2.position.x - p1.position.x) +
                  (p2.position.y - p1.position.y) * (p2.position.y - p1.position.y);

    return 15 + std::max(int(std::sqrt(dist2)), 50);
}

void BrushAction::interpolateAndPaint() {
    if (mousePoints_.size() < 4) return; // Require 4 points for Hermite interpolation

    const auto& p0 = mousePoints_[0];
    const auto& p1 = mousePoints_[1];
    const auto& p2 = mousePoints_[2];
    const auto& p3 = mousePoints_[3];

    int steps = evalNumStepsNeeded(p0, p1, p2, p3);

    for (float i = 1.f; i <= float(steps); ++i) {
        float t =
        paintAtPosition(interpolateHermite(p0, p1, p2, p3, t));
    }

    mousePoints_.pop_front();
}

static float sTime = 0.0f;
constexpr float kTimeStep = 0.01f;

bool BrushAction::operator()(const psapi::IRenderWindow* renderWindow,
                             const psapi::sfm::Event& event) {
    if (!canvas_ || !canvas_->isPressed()) {
        mousePoints_.clear();
        return false;
    }

    psapi::sfm::vec2i currentPos = canvas_->getMousePosition();
    psapi::sfm::vec2f currentPosF = {static_cast<float>(currentPos.x),
                                     static_cast<float>(currentPos.y)};
    float currentTime = sTime;
    sTime += kTimeStep;

    mousePoints_.push_back({currentPosF, currentTime});

    interpolateAndPaint();

    return true;
}

bool loadPlugin() {
    texture = psapi::sfm::Texture::create().release();
    texture->loadFromFile("./assets/buttons.png");

    auto toolbar_sprite = psapi::sfm::Sprite::create();
    toolbar_sprite->setTexture(texture);
    toolbar_sprite->setTextureRect(kBrushButtonTextureArea);

    auto toolbar = dynamic_cast<psapi::IBar*>(psapi::getRootWindow()->getWindowById(psapi::kToolBarWindowId));

    auto brush_action = std::make_unique<BrushAction>();
    auto brush_button = std::make_unique<ps::ABarButton>(std::move(toolbar_sprite), toolbar, std::move(brush_action));
    toolbar->addWindow(std::move(brush_button));

    return true;
}

void unloadPlugin() {
    delete texture;
}
