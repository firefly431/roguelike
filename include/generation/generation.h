#ifndef ROGUELIKE_INCLUDED_GENERATION_GENERATION
#define ROGUELIKE_INCLUDED_GENERATION_GENERATION

namespace generation {
    class Dungeon {
        struct Point {
            unsigned int r, c;
        };
        unsigned int width, height;
        std::vector<int> grid;
        std::vector<Point> roompts;
        std::mt19937 mt;

        bool useless(const Point &c);
        bool touches_merged(const Point &c);
        void floodfill(const Point &c);
    public:
        Dungeon(unsigned int width, unsigned int height, unsigned int nrooms);
        Dungeon(unsigned int width, unsigned int height, unsigned int nrooms, std::mt19937::result_type seed);
        void generate(unsigned int ntries);
        void export(const char *fname);
    };
}

#endif
