#include <cstring>
#include <random>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>

using std::memset;

struct s_point {
    unsigned int y, x;
};

int main(int argc, char **argv) {
    const unsigned int WIDTH = 301, HEIGHT = 201;
    const unsigned int NTRIES = 500, NROOMS = 40;
    int grid[WIDTH * HEIGHT] = {0};
    unsigned int roompts[NROOMS * 2] = {0}; // (y, x)
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
            roompts[2 * nrooms - 2] = y;
            roompts[2 * nrooms - 1] = x;
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
        for (unsigned int y = 1; y < HEIGHT - 1u; y += 2) {
            for (unsigned int x = 1; x < WIDTH - 1u; x += 2) {
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
                                                        turn(0, 5);
            unsigned int sdir = compass(mt);
            // turn directions: 0 (same), 1 (right), 3 (left)
            unsigned int dirs[];
            switch (turn) {
            case 0:
                dirs[0] = 0; dirs[1] = 1; dirs[2] = 3; break;
            case 1:
                dirs[0] = 0; dirs[1] = 3; dirs[2] = 1; break;
            case 2:
                dirs[0] = 1; dirs[1] = 0; dirs[2] = 3; break;
            case 3:
                dirs[0] = 1; dirs[1] = 3; dirs[2] = 0; break;
            case 4:
                dirs[0] = 3; dirs[1] = 0; dirs[2] = 1; break;
            case 5:
                dirs[0] = 3; dirs[1] = 1; dirs[2] = 0; break;
            }
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
    // connect rooms together
    std::vector<s_point> connectors;
    bool merged[WIDTH * HEIGHT] = {0};
    const auto &grid_cap = grid;
    const auto &merged_cap = merged;
    std::ofstream mergelog("mergelog.txt");
    auto useless = [&grid_cap, &merged_cap, WIDTH](const s_point &c) -> bool {
        const auto &grid = grid_cap;
        const auto &merged = merged_cap;
        const unsigned int x = c.x, y = c.y;
        int found1 = 0, found2 = 0;
        bool foundwall = false;
        if (x > 1 && grid[y * WIDTH + x - 1]) {
            if (grid[y * WIDTH + x - 1] > 0)
                if (!merged[y * WIDTH + x - 1])
                    if (found1 > 0) found2 = grid[y * WIDTH + x - 1];
                    else found1 = grid[y * WIDTH + x - 1];
                else foundwall = true;
            else foundwall = true;
        }
        if (found2 == found1) found2 = 0;
        if (y > 1 && grid[(y - 1) * WIDTH + x]) {
            if (grid[(y - 1) * WIDTH + x] > 0)
                if (!merged[(y - 1) * WIDTH + x])
                    if (found1 > 0) found2 = grid[(y - 1) * WIDTH + x];
                    else found1 = grid[(y - 1) * WIDTH + x];
                else foundwall = true;
            else foundwall = true;
        }
        if (found2 == found1) found2 = 0;
        if (x < WIDTH - 1u && grid[y * WIDTH + x + 1]) {
            if (grid[y * WIDTH + x + 1] > 0)
                if (!merged[y * WIDTH + x + 1])
                    if (found1 > 0) found2 = grid[y * WIDTH + x + 1];
                    else found1 = grid[y * WIDTH + x + 1];
                else foundwall = true;
            else foundwall = true;
        }
        if (found2 == found1) found2 = 0;
        if (y < HEIGHT - 1u && grid[(y + 1) * WIDTH + x]) {
            if (grid[(y + 1) * WIDTH + x] > 0)
                if (!merged[(y + 1) * WIDTH + x])
                    if (found1 > 0) found2 = grid[(y + 1) * WIDTH + x];
                    else found1 = grid[(y + 1) * WIDTH + x];
                else foundwall = true;
            else foundwall = true;
        }
        if (found2 == found1) found2 = 0;
        return !(found1 && (found2 || foundwall));
    };
    auto touches_merged = [&merged_cap, WIDTH](const s_point &c) -> bool {
        const auto &merged = merged_cap;
        const unsigned int x = c.x, y = c.y;
        if (x > 1 && merged[y * WIDTH + x - 1]) {
            return true;
        }
        if (y > 1 && merged[(y - 1) * WIDTH + x]) {
            return true;
        }
        if (x < WIDTH - 1u && merged[y * WIDTH + x + 1]) {
            return true;
        }
        if (y < HEIGHT - 1u && merged[(y + 1) * WIDTH + x]) {
            return true;
        }
        return false;
    };
    for (unsigned int y = 0; y < HEIGHT; y++) {
        for (unsigned int x = 0; x < WIDTH; x++) {
            // check if it connects two rooms together
            if (grid[y * WIDTH + x]) continue;
            s_point pt = {y, x};
            if (!useless(pt))
                connectors.push_back(pt);
        }
    }
    std::shuffle(connectors.begin(), connectors.end(), mt);
    bool isconnector[WIDTH * HEIGHT] = {0};
    // merge the first room
    auto floodfill = [&grid_cap, &merged, &mergelog, &touches_merged, WIDTH](const s_point &c) {
        const auto &grid = grid_cap;
        std::queue<s_point> Q;
        Q.push(c);
        while (!Q.empty()) {
            const auto p = Q.front();
            Q.pop();
            merged[p.y * WIDTH + p.x] = true;
            const unsigned int x = p.x, y = p.y;
            // add neighbors
            if (x > 1 && grid[y * WIDTH + x - 1] && !merged[y * WIDTH + x - 1]) {
                merged[y * WIDTH + x - 1] = true;
                Q.emplace(s_point {y, x - 1});
            }
            if (y > 1 && grid[(y - 1) * WIDTH + x] && !merged[(y - 1) * WIDTH + x]) {
                merged[(y - 1) * WIDTH + x] = true;
                Q.emplace(s_point {y - 1, x});
            }
            if (x < WIDTH - 1u && grid[y * WIDTH + x + 1] && !merged[y * WIDTH + x + 1]) {
                merged[y * WIDTH + x + 1] = true;
                Q.emplace(s_point {y, x + 1});
            }
            if (y < HEIGHT - 1u && grid[(y + 1) * WIDTH + x] && !merged[(y + 1) * WIDTH + x]) {
                merged[(y + 1) * WIDTH + x] = true;
                Q.emplace(s_point {y + 1, x});
            }
        }
        s_point pt;
        for (unsigned int i = 0; i < HEIGHT; i++) {
            for (unsigned int j = 0; j < WIDTH; j++) {
                pt.y = i;
                pt.x = j;
                mergelog << (i == c.y && j == c.x ? '*' : merged[i * WIDTH + j] ? '.' : touches_merged(pt) ? '=' : '#');
            }
            mergelog << ' ';
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
                mergelog << c;
            }
            mergelog << std::endl;
        }
        mergelog << std::endl;
    };
    {
        s_point p = {roompts[0], roompts[1]};
        floodfill(p);
    }
    // activate connectors
    while (!connectors.empty()) {
        auto conn_ = std::find_if(connectors.begin(), connectors.end(), touches_merged);
        if (conn_ == connectors.end()) {
            // wtf????? quite certain this is impossible
            break;
        }
        auto conn = *conn_;
        if (!touches_merged(conn)) {
            std::cerr << "WTF" << std::endl;
        }
        connectors.erase(conn_); // probably unnecessary
        grid[conn.y * WIDTH + conn.x] = -1;
        floodfill(conn);
        std::bernoulli_distribution dist(0.0);
        //std::remove_if(connectors.begin(), connectors.end(), [&dist, &mt, &useless](const s_point &p) {return useless(p) && !dist(mt);});
        connectors.erase(std::remove_if(connectors.begin(), connectors.end(), useless), connectors.end());
        memset(isconnector, 0, sizeof isconnector);
        for (const auto &a : connectors) {
            if (isconnector[a.y * WIDTH + a.x]) std::cerr << "Error" << std::endl;
            isconnector[a.y * WIDTH + a.x] = true;
        }
        for (unsigned int i = 0; i < HEIGHT; i++) {
            for (unsigned int j = 0; j < WIDTH; j++) {
                int v = grid[i * WIDTH + j];
                char c;
                if (v == 0)
                    c = isconnector[i * WIDTH + j] ? '=' : '#';
                else if (1 <= v && v <= 9)
                    c = v + '0';
                else if (10 <= v && v <= 35)
                    c = v + 'A' - 10;
                else if (v < 0)
                    c = ' ';
                else
                    c = '?';
                mergelog << c;
            }
            mergelog << std::endl;
        }
        mergelog << std::endl;
        mergelog << "Merges:" << std::endl;
        mergelog << std::endl;
    }
    mergelog.close();
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
