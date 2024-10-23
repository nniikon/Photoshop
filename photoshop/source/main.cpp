#include "api_sfm.hpp"
#include "sfm_implementation.h"
#include "ps_plugin_toolbar.h"

#include "loguru.hpp"
#include <dlfcn.h>

using namespace psapi;

int main(int argc, char *argv[]) {
    loguru::init(argc, argv);
    LOG_F(INFO, "Photoshop started!");

    sfm::RenderWindow window(800, 600, "StandPhotoshop");

    loadPlugin();

    while (window.isOpen()) {
        sfm::Event event;
        if (window.pollEvent(event)) {
            if (event.type == sfm::Event::Closed) {
                window.close();
            }
        }
        window.display();
        window.clear();

        getRootWindow()->draw(&window);
        getRootWindow()->update(&window, event);
    }

    return 0;
}
