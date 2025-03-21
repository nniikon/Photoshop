#include <dlfcn.h>
#include <immintrin.h>

#include "api_sfm.hpp"
#include "sfm_implementation.h"
#include "api_photoshop.hpp"

#include "loguru.hpp"

// loguru violates odr, so disable this
#if __has_feature(address_sanitizer) || defined(__SANITIZE_ADDRESS__)
#ifdef __cplusplus
extern "C"
#endif
const char *__asan_default_options() {
  return "detect_odr_violation=0";
}
#endif

using namespace psapi;

const std::string kPluginPaths[] = {
    "./system_plugins/toolbar/libplugin_toolbar.so",
    "./system_plugins/canvas/libplugin_canvas.so",
    "./system_plugins/brush/libplugin_brush.so",
    "./system_plugins/line/libplugin_line.so",
    "./system_plugins/shape/libplugin_shape.so",
    "./system_plugins/eraser/libplugin_eraser.so",
    "./system_plugins/secondary_panel/libplugin_secondary_panel.so",
    "./system_plugins/filters/libplugin_filters.so",
};

int onLoadPlugins();

int main(int argc, char *argv[]) {
    loguru::init(argc, argv);

    sfm::RenderWindow window(1200, 1080, "Photoshop");

    int err = onLoadPlugins();
    if (err) {
        LOG_F(ERROR, "Can't load plugins");
        return 1;
    }

    auto action_controller = psapi::getActionController();

    while (window.isOpen()) {
        sfm::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sfm::Event::Closed) {
                window.close();
            }
            action_controller->execute(getRootWindow()->createAction(&window, event));
        }

        sfm::Event idle_event = {.type = sfm::Event::None};

        action_controller->execute(getRootWindow()->createAction(&window, idle_event));
        getRootWindow()->draw(&window);

        window.display();
        window.clear();
    }

    return 0;
}

int onLoadPlugins() {
    for (const auto& path : kPluginPaths) {
        void* lib = dlopen(path.c_str(), RTLD_LAZY);
        if (!lib) {
            LOG_F(ERROR, "Can't load plugin: %s", dlerror());
            return 1;
        }

        auto onLoadPlugin = (bool (*)(void))dlsym(lib, "onLoadPlugin");
        if (!onLoadPlugin) {

            LOG_F(ERROR, "Can't load plugin: %s", dlerror());
            return 1;
        }

        onLoadPlugin();
    }

    return 0;
}
