#ifndef ROGUELIKE_INCLUDED_GENERATION_GENERATION
#define ROGUELIKE_INCLUDED_GENERATION_GENERATION

#include <random>
#include <vector>

namespace generation {
    class Dungeon {
        struct Point {
            unsigned int r, c;
        };
        unsigned int width, height;
        std::vector<int> grid;
        std::vector<bool> merged;
        std::vector<Point> roompts;
        std::mt19937 mt;

        bool useless(const Point &c) const;
        bool touches_merged(const Point &c);
        bool touches_cleared(const Point &c);
        void floodfill(const Point &c);
    public:
        Dungeon(unsigned int width, unsigned int height);
        Dungeon(unsigned int width, unsigned int height, std::mt19937::result_type seed);
        void generate(unsigned int nrooms, unsigned int ntries, double extra_door_p = 0.05, double dead_end_p = 0.05, double straight_p = 0.5, double straight_second_p = 0.67);
        void write(const char *fname);
    };
}

#endif
