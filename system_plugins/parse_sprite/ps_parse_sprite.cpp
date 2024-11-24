#include "ps_parse_sprite.h"

#include <sys/stat.h>
#include <fstream>

#include "loguru.hpp"

using namespace ps;

static size_t GetFileSize(const std::string& filename);
static std::string ReadFileToString(const std::string& filename);
static SpriteInfo ParseConfigFromContent(const std::string& file_content);

static size_t GetFileSize(const std::string& filename) {
    struct stat file_stat;
    if (stat(filename.c_str(), &file_stat) != 0) {
        throw std::runtime_error(std::string("Error getting file size: ") + filename);
    }

    return (size_t)file_stat.st_size;
}

static std::string ReadFileToString(const std::string& filename) {
    size_t file_size = GetFileSize(filename);

    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error(std::string("Error opening file: ") + filename);
    }

    std::string file_content(file_size, '\0');
    file.read(&file_content[0], (long)file_size);
    if (!file) {
        throw std::runtime_error(std::string("Error reading file: ") + filename);
    }

    return file_content;
}

static SpriteInfo ParseConfigFromContent(const std::string& file_content) {
    SpriteInfo info = {};
    vec2i screen_pos{};
    vec2i image_pos{};
    vec2i image_size{};

    char image_path_buffer[128] = {};
    char name_buffer[64]        = {};
    char descr_buffer[1024]     = {};

    int result = std::sscanf(file_content.c_str(),
        "SCREEN_POS:" " {%d, %d}" "\n"
        "IMAGE_POS:"  " {%d, %d}" "\n"
        "IMAGE_SIZE:" " {%d, %d}" "\n"
        "IMAGE_PATH:" " %127s"    "\n"
        "NAME:"       " %63s"     "\n"
        "DESCRIPTION:"" %1023[^\n]s",
        &screen_pos.x, &screen_pos.y,
        &image_pos .x,  &image_pos.y,
        &image_size.x, &image_size.y,
        image_path_buffer,
        name_buffer,
        descr_buffer
    );

    if (result != 9) {
        throw std::runtime_error(std::string("Error parsing config: ") + file_content);
    }

    std::unique_ptr<ITexture> texture = psapi::sfm::ITexture::create();
    texture->loadFromFile(image_path_buffer);

    std::unique_ptr<ISprite> sprite = psapi::sfm::ISprite::create();
    sprite->setTexture(texture.get());

    psapi::sfm::IntRect rect = {image_pos.x, image_pos.y, image_size.x, image_size.y};
    sprite->setTextureRect(rect);

    info.texture = std::move(texture);
    info.sprite  = std::move(sprite);
    info.image_path  = std::string(image_path_buffer);
    info.name        = std::string( name_buffer);
    info.description = std::string(descr_buffer);

    // Dump everything:
    LOG_F(INFO, "Found button config:");
    LOG_F(INFO, "\t\t" "SCREEN_POS: {%d, %d}",  screen_pos.x, screen_pos.y);
    LOG_F(INFO, "\t\t" "IMAGE_POS:  {%d, %d}",  image_pos.x, image_pos.y);
    LOG_F(INFO, "\t\t" "IMAGE_SIZE: {%d, %d}",  image_size.x, image_size.y);
    LOG_F(INFO, "\t\t" "IMAGE_PATH: %s",        info.image_path.c_str());
    LOG_F(INFO, "\t\t" "NAME:       %s",        info.name.c_str());
    LOG_F(INFO, "\t\t" "DESCRIPTION:%s",        info.description.c_str());

    return info;
}

namespace ps {

SpriteInfo ParseSpriteFromConfig(const std::string& config_path) {
    std::string file_content = ReadFileToString(config_path);
    return ParseConfigFromContent(file_content);
}

} // namespace
