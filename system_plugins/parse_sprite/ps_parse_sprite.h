#include <api_sfm.hpp>

#include <string>

using psapi::sfm::vec2i;
using psapi::sfm::ITexture;
using psapi::sfm::ISprite;

namespace ps {

struct SpriteInfo {
    std::unique_ptr<ITexture> texture;
    std::unique_ptr<ISprite> sprite;
    std::string image_path;
    std::string name;
    std::string description;
};

SpriteInfo ParseSpriteFromConfig(const std::string& config_path);

} // namespace
