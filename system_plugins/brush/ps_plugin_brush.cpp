#include "ps_plugin_brush.h"

#include "loguru.hpp"
#include "sfm_implementation.h"
#include "ps_plugin_secondary_panel.h"

#include <cmath>

const psapi::sfm::IntRect kBrushButtonTextureArea = {{0, 0}, {64, 64}};

constexpr int kBrushRadius = 1;
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
                    layer->setPixel(pixel_pos, color_);
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
    // LOG_F(INFO, "mouse points: p0(%f, %f), p1(%f, %f), p2(%f, %f), p3(%f, %f), t=%f",
    //        p0.position.x, p0.position.y, p1.position.x, p1.position.y,
    //        p2.position.x, p2.position.y, p3.position.x, p3.position.y,
    //        t);

    float t2 = t * t;
    float t3 = t2 * t;

    float h00 = 2 * t3 - 3 * t2 + 1;
    float h10 = t3 - 2 * t2 + t;
    float h01 = -2 * t3 + 3 * t2;
    float h11 = t3 - t2;

    float m0x = (p2.position.x - p0.position.x) / 2.f;
    float m0y = (p2.position.y - p0.position.y) / 2.f;

    float m1x = (p3.position.x - p1.position.x) / 2.f;
    float m1y = (p3.position.y - p1.position.y) / 2.f;

    float x = h00 * p1.position.x + m0x * h10 + p2.position.x * h01 + m1x * h11;
    float y = h00 * p1.position.y + m0y * h10 + p2.position.y * h01 + m1y * h11;

    return {static_cast<int>(std::round(x)), static_cast<int>(std::round(y))};
}

int BrushAction::evalNumStepsNeeded(const MousePoint& p0,
                                    const MousePoint& p1,
                                    const MousePoint& p2,
                                    const MousePoint& p3) {
    float dist2 = (p2.position.x - p1.position.x) * (p2.position.x - p1.position.x) +
                  (p2.position.y - p1.position.y) * (p2.position.y - p1.position.y);

    return 0 + std::max(int(std::sqrt(dist2)), 50);
}

void BrushAction::interpolateAndPaint() {
    if (mouse_points_.size() < 4)
        return;

    const auto& p0 = mouse_points_[0];
    const auto& p1 = mouse_points_[1];
    const auto& p2 = mouse_points_[2];
    const auto& p3 = mouse_points_[3];

    int steps = evalNumStepsNeeded(p0, p1, p2, p3);

    for (float i = 1.f; i <= float(steps); ++i) {
        float t = float(i) / float(steps);
        paintAtPosition(interpolateHermite(p0, p1, p2, p3, t));
    }

    mouse_points_.pop_front();
}

static float sTime = 0.0f;
constexpr float kTimeStep = 0.00001f;

bool BrushAction::operator()(const psapi::IRenderWindow* renderWindow,
                             const psapi::sfm::Event& event) {
    if (!canvas_ || !canvas_->isPressed()) {
        mouse_points_.clear();
        return false;
    }

    auto secondary_panel = dynamic_cast<const ps::SecondaryPanel*>(psapi::getRootWindow()->getWindowById(ps::kSecondaryPanelWindowId));
    color_ = secondary_panel->getColor();

    psapi::sfm::vec2i current_pos = canvas_->getMousePosition();

    if ((last_mouse_position_.x - current_pos.x) * (last_mouse_position_.x - current_pos.x) +
        (last_mouse_position_.y - current_pos.y) * (last_mouse_position_.y - current_pos.y) < 5) {
        return true;
    }

    last_mouse_position_ = current_pos;

    psapi::sfm::vec2f current_pos_f = {static_cast<float>(current_pos.x),
                                       static_cast<float>(current_pos.y)};

    float current_time = sTime;
    sTime += kTimeStep;

    mouse_points_.push_back({current_pos_f, current_time});

    interpolateAndPaint();

    return true;
}

bool BrushAction::activate() {
    return true;
}

bool loadPlugin() {
    texture = psapi::sfm::ITexture::create().release(); // raii rip
    texture->loadFromFile("./assets/buttons.png");

    auto toolbar_sprite = psapi::sfm::Sprite::create();
    toolbar_sprite->setTexture(texture);
    toolbar_sprite->setTextureRect(kBrushButtonTextureArea);

    auto toolbar = dynamic_cast<psapi::IBar*>(
        psapi::getRootWindow()->getWindowById(psapi::kToolBarWindowId)
    );

    auto brush_action = std::make_unique<BrushAction>();
    auto brush_button = std::make_unique<ps::ABarButton>(
        std::move(toolbar_sprite),
        toolbar,
        std::move(brush_action)
    );

    toolbar->addWindow(std::move(brush_button));

    return true;
}

void unloadPlugin() {
    delete texture;
}
