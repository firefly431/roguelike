#include <random>
#include <iostream>
#include <fstream>

int main(int argc, char **argv) {
    const unsigned int WIDTH = 31, HEIGHT = 21;
    const unsigned int NTRIES = 500, NROOMS = 10;
    int grid[WIDTH * HEIGHT] = {0};
    // random
    std::mt19937 mt((std::random_device()()));
    // 0 = wall, 1+ = room, -1 = corridor
    // generate rooms
    unsigned int nrooms = 0;
    std::uniform_int_distribution<int> sdist(1, 5);
    for (unsigned int i = 0; i < NTRIES && nrooms < NROOMS; i++) {
        while (nrooms < NROOMS) {
            // generate a random rectangle
            int width = sdist(mt) * 2 + 1;
            int height = sdist(mt) * 2 + 1;
            std::uniform_int_distribution<int> xpos(0, WIDTH / 2 - width / 2 - 1),
                                               ypos(0, HEIGHT / 2 - height / 2 - 1);
            int x = xpos(mt) * 2 + 1;
            int y = ypos(mt) * 2 + 1;
            for (int j = 0; j < height; j++) {
                for (int k = 0; k < width; k++) {
                    if (grid[(y + j) * WIDTH + x + k])
                        goto overlap;
                }
            }
            nrooms++;
            for (int j = 0; j < height; j++) {
                for (int k = 0; k < width; k++) {
                    grid[(y + j) * WIDTH + x + k] = nrooms;
                }
            }
        }
overlap:;
    }
    // fill with corridors
    // pick a random spot
    while (true) {
        std::uniform_int_distribution<int> xpos(0, WIDTH / 2 - 1),
                                           ypos(0, HEIGHT / 2 - 1);
        unsigned int x = xpos(mt) * 2 + 1, y = ypos(mt) * 2 + 1;
        if (!grid[y * WIDTH + x]) {
            grid[y * WIDTH + x] = -1;
            break;
        }
    }
    // hunt and kill
    while (true) {
        unsigned int cx = 0, cy = 0;
        // find an empty cell with a visited neighbor
        std::cout << "Hunting" << std::endl;
        for (unsigned int y = 1; y < HEIGHT - 1; y += 2) {
            for (unsigned int x = 1; x < WIDTH - 1; x += 2) {
                if (!grid[y * WIDTH + x]) {
                    if (x > 1 && grid[y * WIDTH + x - 2] == -1)
                        grid[y * WIDTH + x - 1] = -1;
                    else if (y > 1 && grid[(y - 2) * WIDTH + x] == -1)
                        grid[(y - 1) * WIDTH + x] = -1;
                    else if (x < WIDTH - 2 && grid[y * WIDTH + x + 2] == -1)
                        grid[y * WIDTH + x + 1] = -1;
                    else if (y < HEIGHT - 2 && grid[(y + 2) * WIDTH + x] == -1)
                        grid[(y + 1) * WIDTH + x] = -1;
                    else continue;
                    cx = x;
                    cy = y;
                    goto found;
                }
            }
        }
found:;
        if (cx == 0) break; // we're done; not found (cx is always odd)
        while (true) {
            // clear ourselves
            grid[cy * WIDTH + cx] = -1;
            // advance in random direction
            std::uniform_int_distribution<unsigned int> compass(0, 3),
                                                        turn(1, 3);
            unsigned int sdir = compass(mt);
            // turn directions: 0 (same), 1 (right), 3 (left)
            unsigned int dirs[] = {turn(mt), 0, 0};
            unsigned int &first = dirs[0],
                         &second = dirs[1],
                         &third = dirs[2];
            if (first == 2) first = 0;
            first = (first + sdir) & 3;
            second = first ^ turn(mt); // flip at least 1 bit
            third = (4 - (first - sdir) - (second - sdir)) & 3;
            bool found = false;
            for (int i = 0; i < 3; i++) {
                int dir = dirs[i];
                switch (dir) {
                case 0: // north
                    if (cy > 1 && !grid[(cy - 2) * WIDTH + cx]) {
                        grid[(cy - 1) * WIDTH + cx] = -1;
                        cy -= 2;
                        found = true;
                    }
                    break;
                case 1: // east
                    if (cx < WIDTH - 2 && !grid[cy * WIDTH + cx + 2]) {
                        grid[cy * WIDTH + cx + 1] = -1;
                        cx += 2;
                        found = true;
                    }
                    break;
                case 2: // south
                    if (cy < HEIGHT - 2 && !grid[(cy + 2) * WIDTH + cx]) {
                        grid[(cy + 1) * WIDTH + cx] = -1;
                        cy += 2;
                        found = true;
                    }
                    break;
                case 3: // west
                    if (cx > 1 && !grid[cy * WIDTH + cx - 2]) {
                        grid[cy * WIDTH + cx - 1] = -1;
                        cx -= 2;
                        found = true;
                    }
                    break;
                }
                if (found) break;
            }
            if (!found) break; // no neighbors
        }
    }
    // TODO: connect rooms together
    // output maze
    std::ofstream file("maze.txt");
    for (unsigned int i = 0; i < HEIGHT; i++) {
        for (unsigned int j = 0; j < WIDTH; j++) {
            int v = grid[i * WIDTH + j];
            char c;
            if (v == 0)
                c = '#';
            else if (1 <= v && v <= 9)
                c = v + '0';
            else if (10 <= v && v <= 35)
                c = v + 'A' - 10;
            else if (v < 0)
                c = ' ';
            else
                c = '?';
            file << c;
        }
        file << std::endl;
    }
    file.close();
    return 0;
}
