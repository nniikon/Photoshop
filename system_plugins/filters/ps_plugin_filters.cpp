#include "ps_plugin_filters.h"

#include "ps_parse_sprite.h"
#include "api_canvas.hpp"
#include <cassert>

static psapi::sfm::ITexture* neg_filter_texture = nullptr;
static psapi::sfm::ITexture* emboss_filter_texture = nullptr;
static psapi::sfm::ITexture* blur_filter_texture = nullptr;

FilterNegAction::FilterNegAction() {
    canvas_ = static_cast<psapi::ICanvas*>(
        psapi::getRootWindow()->getWindowById(psapi::kCanvasWindowId)
    );
}

bool FilterNegAction::operator()(const psapi::IRenderWindow* renderWindow,
                                 const psapi::sfm::Event& event) {
    return false;
}

bool FilterNegAction::activate() {
    if (!canvas_)
        return false;

    psapi::vec2i size = {static_cast<int>(canvas_->getSize().x),
                         static_cast<int>(canvas_->getSize().y)};
    psapi::ILayer* layer = canvas_->getLayer(canvas_->getActiveLayerIndex());

    for (int y = 0; y < size.y; y++) {
        for (int x = 0; x < size.x; x++) {
            psapi::sfm::Color pixel = layer->getPixel({x, y});
            psapi::sfm::Color negative_pixel = {
                255 - pixel.r,
                255 - pixel.g,
                255 - pixel.b
            };

            layer->setPixel({x, y}, negative_pixel);
        }
    }
    return false;
}

EmbossAction::EmbossAction() {
    canvas_ = static_cast<psapi::ICanvas*>(
        psapi::getRootWindow()->getWindowById(psapi::kCanvasWindowId)
    );
}

bool EmbossAction::operator()(const psapi::IRenderWindow* renderWindow,
                              const psapi::sfm::Event& event) {
    return false;
}

bool EmbossAction::activate() {
    if (!canvas_)
        return false;

    psapi::vec2i size = {static_cast<int>(canvas_->getSize().x),
                         static_cast<int>(canvas_->getSize().y)};
    psapi::ILayer* layer = canvas_->getLayer(canvas_->getActiveLayerIndex());

    for (int y = 0; y < size.y; y++) {
        for (int x = 0; x < size.x; x++) {
            psapi::sfm::Color pixel = layer->getPixel({x, y});

            const int offset = 2;
            psapi::sfm::Color shifted_pixel = layer->getPixel({x, y});
            if (x + offset < size.x && y + offset < size.y)
                shifted_pixel = layer->getPixel({x + offset,
                                                 y + offset});

            psapi::sfm::Color negative_pixel = {
                255 - shifted_pixel.r,
                255 - shifted_pixel.g,
                255 - shifted_pixel.b
            };

            psapi::sfm::Color emboss_pixel = {
                (pixel.r + negative_pixel.r) / 2,
                (pixel.g + negative_pixel.g) / 2,
                (pixel.b + negative_pixel.b) / 2
            };

            layer->setPixel({x, y}, emboss_pixel);
        }
    }
    return false;
}

BlurAction::BlurAction() {
    canvas_ = static_cast<psapi::ICanvas*>(
        psapi::getRootWindow()->getWindowById(psapi::kCanvasWindowId)
    );
}

bool BlurAction::operator()(const psapi::IRenderWindow* renderWindow,
                             const psapi::sfm::Event& event) {
    return false;
}


void BlurAction::convolve(const float* kernel, psapi::vec2i kernel_size) {
    assert(kernel_size.x % 2 == 1);
    assert(kernel_size.y % 2 == 1);

    psapi::vec2i image_size = {static_cast<int>(canvas_->getSize().x),
                               static_cast<int>(canvas_->getSize().y)};

    psapi::ILayer* old_layer = canvas_->getLayer(canvas_->getActiveLayerIndex());

    std::vector<psapi::sfm::Color> new_layer(static_cast<size_t>(image_size.x * image_size.y));

    for (int y = 0; y < image_size.y; y++) {
        for (int x = 0; x < image_size.x; x++) {
            float new_r = 0;
            float new_g = 0;
            float new_b = 0;
            float new_a = old_layer->getPixel({x, y}).a;

            for (int ky = -(kernel_size.y / 2); ky <= (kernel_size.y / 2); ky++) {
                for (int kx = -(kernel_size.x / 2); kx <= (kernel_size.x / 2); kx++) {

                    int kernel_index = (ky + (kernel_size.y / 2)) * kernel_size.x + (kx + (kernel_size.x / 2));

                    int new_x = x + kx;
                    int new_y = y + ky;
                    if (new_x < 0)             new_x = 0;
                    if (new_x >= image_size.x) new_x = image_size.x - 1;
                    if (new_y < 0)             new_y = 0;
                    if (new_y >= image_size.y) new_y = image_size.y - 1;

                    new_r += (kernel[kernel_index] * old_layer->getPixel({new_x, new_y}).r);
                    new_g += (kernel[kernel_index] * old_layer->getPixel({new_x, new_y}).g);
                    new_b += (kernel[kernel_index] * old_layer->getPixel({new_x, new_y}).b);
                }
            }

            if (new_r > 255.f) new_r = 255.f;
            if (new_g > 255.f) new_g = 255.f;
            if (new_b > 255.f) new_b = 255.f;

            if (new_r < 0.f) new_r = 0.f;
            if (new_g < 0.f) new_g = 0.f;
            if (new_b < 0.f) new_b = 0.f;

            new_layer[static_cast<size_t>(y * image_size.x + x)] = psapi::sfm::Color{(uint8_t)new_r,
                                                                                     (uint8_t)new_g,
                                                                                     (uint8_t)new_b,
                                                                                     (uint8_t)new_a};
        }
    }

    for (int y = 0; y < image_size.y; y++) {
        for (int x = 0; x < image_size.x; x++) {
            old_layer->setPixel({x, y}, new_layer[static_cast<size_t>(y * image_size.x + x)]);
        }
    }
}

const float gaussian_kernel[25] = {
     1.f / 273.f,  4.f / 273.f,  7.f / 273.f,  4.f / 273.f, 1.f / 273.f,
     4.f / 273.f, 16.f / 273.f, 26.f / 273.f, 16.f / 273.f, 4.f / 273.f,
     7.f / 273.f, 26.f / 273.f, 41.f / 273.f, 26.f / 273.f, 7.f / 273.f,
     4.f / 273.f, 16.f / 273.f, 26.f / 273.f, 16.f / 273.f, 4.f / 273.f,
     1.f / 273.f,  4.f / 273.f,  7.f / 273.f,  4.f / 273.f, 1.f / 273.f
};

const float edge_detection_kernel[9] = {
    -1.f, -1.f, -1.f,
    -1.f,  8.f, -1.f,
    -1.f, -1.f, -1.f
};

bool BlurAction::activate() { 
    if (!canvas_)
        return false;

    psapi::vec2i size = {static_cast<int>(canvas_->getSize().x),
                         static_cast<int>(canvas_->getSize().y)};

    convolve(edge_detection_kernel, {3, 3});
    return false;
}

bool onLoadPlugin() {
    ps::SpriteInfo sprite_info = ps::ParseSpriteFromConfig("system_plugins/filters/ps_plugin_filter_negative_config.pscfg");
    neg_filter_texture = sprite_info.texture.release();

    auto toolbar = dynamic_cast<psapi::IBar*>(psapi::getRootWindow()->getWindowById(psapi::kToolBarWindowId));

    auto neg_action = std::make_unique<FilterNegAction>();
    auto neg_button = std::make_unique<ps::ABarButton>(std::move(sprite_info.sprite),
                                                       toolbar,
                                                       std::move(neg_action));

    toolbar->addWindow(std::move(neg_button));

    sprite_info = ps::ParseSpriteFromConfig("system_plugins/filters/ps_plugin_filter_negative_config.pscfg");
    emboss_filter_texture = sprite_info.texture.release();

    auto emboss_action = std::make_unique<EmbossAction>();
    auto emboss_button = std::make_unique<ps::ABarButton>(std::move(sprite_info.sprite),
                                                          toolbar,
                                                          std::move(emboss_action));
    toolbar->addWindow(std::move(emboss_button));

    sprite_info = ps::ParseSpriteFromConfig("system_plugins/filters/ps_plugin_filter_negative_config.pscfg");
    blur_filter_texture = sprite_info.texture.release();

    auto blur_action = std::make_unique<BlurAction>();
    auto blur_button = std::make_unique<ps::ABarButton>(std::move(sprite_info.sprite),
                                                        toolbar,
                                                        std::move(blur_action));
    toolbar->addWindow(std::move(blur_button));

    return true;
}

void onUnloadPlugin() {
    delete neg_filter_texture;
    delete emboss_filter_texture;
    delete blur_filter_texture;
}
