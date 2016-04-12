#ifndef ROGUELIKE_INCLUDED_GAME_GAME_H
#define ROGUELIKE_INCLUDED_GAME_GAME_H

#include <components/all.h>

#include <vector>

namespace game {
    class Game {
    public:
        typedef unsigned int ComponentMask;
    private:
        // entities
        std::vector<ComponentMask> c_mask;
        // components
        std::vector<GraphicsComponent> c_graphics;
    };
}

#endif
