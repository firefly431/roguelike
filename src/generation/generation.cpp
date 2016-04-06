#include <generation/generation.h>

namespace generation {
    typedef Dungeon::Point Point;

    Dungeon::Dungeon(unsigned int width, unsigned int height, unsigned int nrooms, std::mt19937::result_type seed) : width(width), height(height), grid(width * height, 0), roompts(nrooms), mt(seed) {}
    Dungeon::Dungeon(unsigned int width, unsigned int height, unsigned int nrooms) : this(width, height, nrooms, std::random_device()());

    void Dungeon::export(const char *fname) {
        // output maze
        std::ofstream file(fname);
        for (unsigned int r = 0; r < height; r++) {
            for (unsigned int c = 0; c < width; c++) {
                int v = grid[r * WIDTH + c];
                char ch;
                if (v == 0)
                    ch = '#';
                else if (1 <= v && v <= 9)
                    ch = v + '0';
                else if (10 <= v && v <= 35)
                    ch = v + 'A' - 10;
                else if (v < 0)
                    ch = ' ';
                else
                    ch = '?';
                file << ch;
            }
            file << std::endl;
        }
        file.close();
    }
}
