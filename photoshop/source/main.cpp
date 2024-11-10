#include <dlfcn.h>
#include <immintrin.h>

#include "api_sfm.hpp"
#include "sfm_implementation.h"
#include "api_photoshop.hpp"

#include "loguru.hpp"

using namespace psapi;

const std::string kPluginPaths[] = {
    "./system_plugins/toolbar/libplugin_toolbar.so",
    "./system_plugins/canvas/libplugin_canvas.so",
    "./system_plugins/brush/libplugin_brush.so",
    "./system_plugins/line/libplugin_line.so",
    "./system_plugins/shape/libplugin_shape.so",
    "./system_plugins/eraser/libplugin_eraser.so",
};

int loadPlugins();

int main(int argc, char *argv[]) {
    loguru::init(argc, argv);

    sfm::RenderWindow window(1200, 1080, "Photoshop");

    int err = loadPlugins();
    if (err) {
        LOG_F(ERROR, "Can't load plugins");
        return 1;
    }

    while (window.isOpen()) {
        sfm::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sfm::Event::Closed) {
                window.close();
            }
            getRootWindow()->update(&window, event);
        }

        sfm::Event idle_event = {.type = sfm::Event::None};
        getRootWindow()->update(&window, idle_event);
        getRootWindow()->draw(&window);

        window.display();
        window.clear();
    }

    return 0;
}

int loadPlugins() {
    for (const auto& path : kPluginPaths) {
        void* lib = dlopen(path.c_str(), RTLD_LAZY);
        if (!lib) {
            LOG_F(ERROR, "Can't load plugin: %s", dlerror());
            return 1;
        }

        auto loadPlugin = (bool (*)(void))dlsym(lib, "loadPlugin");
        if (!loadPlugin) {

            LOG_F(ERROR, "Can't load plugin: %s", dlerror());
            return 1;
        }

        loadPlugin();
    }

    return 0;
}
