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
    public:
        Game(unsigned int capacity = 100);

        unsigned int addEntity(); // returns the index of an empty entity
        void removeEntity(unsigned int index);
    };
}

#endif
