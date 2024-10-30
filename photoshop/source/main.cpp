#include <dlfcn.h>
#include <immintrin.h>

#include "api_sfm.hpp"
#include "sfm_implementation.h"
#include "api_photoshop.hpp"

#include "loguru.hpp"

using namespace psapi;

const std::string kPluginPaths[] = {
    "./system_plugins/libplugin_toolbar.so",
    "./system_plugins/libplugin_canvas.so",
    "./system_plugins/libplugin_brush.so",
    "./system_plugins/libplugin_line.so",
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
        uint64_t time1 = __rdtsc();
        sfm::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sfm::Event::Closed) {
                window.close();
            }
            getRootWindow()->update(&window, event);
        }

        getRootWindow()->update(&window, {.type = sfm::Event::None});
        getRootWindow()->draw(&window);

        uint64_t time2 = __rdtsc();
        window.display();
        window.clear();
        uint64_t time3 = __rdtsc();


        // LOG_F(INFO, "Business: %f%%", float(time2 - time1) / float(time3 - time1) * 100.f);
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
