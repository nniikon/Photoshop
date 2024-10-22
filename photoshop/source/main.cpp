#include "api_sfm.hpp"
#include "sfm_implementation.h"

#include "loguru.hpp"

using namespace psapi;

int main(int argc, char *argv[]) {
    loguru::init(argc, argv);
    LOG_F(INFO, "Photoshop started!");

    sfm::RenderWindow window(800, 600, "StandPhotoshop");

    while (window.isOpen()) {
        sfm::Event event;
        if (window.pollEvent(event)) {
            if (event.type == sfm::Event::Closed) {
                window.close();
            }
        }
    }

    return 0;
}
