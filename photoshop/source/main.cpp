#include "api_sfm.hpp"
#include "sfm_implementation.h"

using namespace psapi;

int main() {

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
