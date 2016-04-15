#include <game/game.h>

#include <algorithm>

namespace game {
    Game::Game(unsigned int capacity) : c_mask(capacity), c_graphics(capacity) {}

    unsigned int Game::addEntity() {
        if (c_mask.size() < c_mask.capacity()) {
            return c_mask.size();
        }
        // try to find a blank entity
        auto i = std::find(c_mask.begin(), c_mask.end(), 0);
        if (i != c_mask.begin()) {
            return std::distance(c_mask.begin(), i);
        } else {
            c_mask.emplace_back();
            // resize all component arrays
            c_graphics.emplace_back();
        }
    }
    void Game::removeEntity(unsigned int index) {
        c_mask[index] = 0;
        if (index == c_mask.size() - 1) {
            c_mask.pop_back();
            // resize allcomponent arrays
            c_graphics.pop_back();
        }
    }
}
