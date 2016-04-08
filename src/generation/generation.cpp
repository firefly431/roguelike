#include <generation/generation.h>

#include <algorithm>
#include <fstream>
#include <queue>

namespace generation {
    Dungeon::Dungeon(unsigned int width, unsigned int height, std::mt19937::result_type seed) : width(width), height(height), grid(width * height, 0), merged(width * height, 0), roompts(), mt(seed) {}
    Dungeon::Dungeon(unsigned int width, unsigned int height) : Dungeon(width, height, std::random_device()()) {}

    bool Dungeon::useless(const Point &p) const {
        const unsigned int r = p.r, c = p.c;
        int found1 = 0, found2 = 0;
        bool foundwall = false;
        if (c > 1 && grid[r * width + c - 1]) {
            if (grid[r * width + c - 1] > 0)
                if (!merged[r * width + c - 1])
                    if (found1 > 0) found2 = grid[r * width + c - 1];
                    else found1 = grid[r * width + c - 1];
                else foundwall = true;
            else foundwall = true;
        }
        if (found2 == found1) found2 = 0;
        if (r > 1 && grid[(r - 1) * width + c]) {
            if (grid[(r - 1) * width + c] > 0)
                if (!merged[(r - 1) * width + c])
                    if (found1 > 0) found2 = grid[(r - 1) * width + c];
                    else found1 = grid[(r - 1) * width + c];
                else foundwall = true;
            else foundwall = true;
        }
        if (found2 == found1) found2 = 0;
        if (c < width - 1u && grid[r * width + c + 1]) {
            if (grid[r * width + c + 1] > 0)
                if (!merged[r * width + c + 1])
                    if (found1 > 0) found2 = grid[r * width + c + 1];
                    else found1 = grid[r * width + c + 1];
                else foundwall = true;
            else foundwall = true;
        }
        if (found2 == found1) found2 = 0;
        if (r < height - 1u && grid[(r + 1) * width + c]) {
            if (grid[(r + 1) * width + c] > 0)
                if (!merged[(r + 1) * width + c])
                    if (found1 > 0) found2 = grid[(r + 1) * width + c];
                    else found1 = grid[(r + 1) * width + c];
                else foundwall = true;
            else foundwall = true;
        }
        if (found2 == found1) found2 = 0;
        return !(found1 && (found2 || foundwall));
    }

    bool Dungeon::touches_merged(const Point &p) {
        const unsigned int r = p.r, c = p.c;
        if (c > 1 && merged[r * width + c - 1]) {
            return true;
        }
        if (r > 1 && merged[(r - 1) * width + c]) {
            return true;
        }
        if (c < width - 1u && merged[r * width + c + 1]) {
            return true;
        }
        if (r < height - 1u && merged[(r + 1) * width + c]) {
            return true;
        }
        return false;
    }

    bool Dungeon::touches_cleared(const Point &p) {
        const unsigned int r = p.r, c = p.c;
        if (c > 1 && grid[r * width + c - 1] == -2) {
            return true;
        }
        if (r > 1 && grid[(r - 1) * width + c] == -2) {
            return true;
        }
        if (c < width - 1u && grid[r * width + c + 1] == -2) {
            return true;
        }
        if (r < height - 1u && grid[(r + 1) * width + c] == -2) {
            return true;
        }
        return false;
    }

    void Dungeon::floodfill(const Point &q) {
        std::queue<Point> Q;
        Q.push(q);
        while (!Q.empty()) {
            const auto p = Q.front();
            Q.pop();
            merged[p.r * width + p.c] = true;
            const unsigned int r = p.r, c = p.c;
            // add neighbors
            if (c > 1 && grid[r * width + c - 1] && !merged[r * width + c - 1]) {
                merged[r * width + c - 1] = true;
                Q.emplace(Point {r, c - 1});
            }
            if (r > 1 && grid[(r - 1) * width + c] && !merged[(r - 1) * width + c]) {
                merged[(r - 1) * width + c] = true;
                Q.emplace(Point {r - 1, c});
            }
            if (c < width - 1u && grid[r * width + c + 1] && !merged[r * width + c + 1]) {
                merged[r * width + c + 1] = true;
                Q.emplace(Point {r, c + 1});
            }
            if (r < height - 1u && grid[(r + 1) * width + c] && !merged[(r + 1) * width + c]) {
                merged[(r + 1) * width + c] = true;
                Q.emplace(Point {r + 1, c});
            }
        }
    }

    void Dungeon::write(const char *fname) {
        // output maze
        std::ofstream file(fname);
        for (unsigned int r = 0; r < height; r++) {
            for (unsigned int c = 0; c < width; c++) {
                int v = grid[r * width + c];
                char ch;
                if (v == 0)
                    ch = '#';
                else if (1 <= v && v <= 9)
                    ch = v + '0';
                else if (10 <= v && v <= 35)
                    ch = v + 'A' - 10;
                else if (v == -1)
                    ch = '.';
                else if (v == -2)
                    ch = ':';
                else if (v == -3)
                    ch = '!';
                else
                    ch = '?';
                file << ch;
            }
            file << std::endl;
        }
        file.close();
    }

    void Dungeon::generate(unsigned int nrooms, unsigned int ntries, double extra_door_p, double dead_end_p) {
        // 0 = wall, 1+ = room, -1 = corridor, -2 = door, -3 = dead end
        // generate rooms
        std::uniform_int_distribution<unsigned int> sdist(1, 5);
        for (unsigned int i = 0; i < ntries && roompts.size() < nrooms; i++) {
            while (roompts.size() < nrooms) {
                // generate a random rectangle
                unsigned int rwidth = sdist(mt) * 2 + 1;
                unsigned int rheight = sdist(mt) * 2 + 1;
                std::uniform_int_distribution<int> rpos(0, height / 2 - rheight / 2 - 1),
                                                   cpos(0, width / 2 - rwidth / 2 - 1);
                unsigned int r = rpos(mt) * 2 + 1;
                unsigned int c = cpos(mt) * 2 + 1;
                for (unsigned int j = 0; j < rheight; j++) {
                    for (unsigned int k = 0; k < rwidth; k++) {
                        if (grid[(r + j) * width + c + k])
                            goto overlap;
                    }
                }
                roompts.emplace_back(Point {r, c});
                for (unsigned int j = 0; j < rheight; j++) {
                    for (unsigned int k = 0; k < rwidth; k++) {
                        grid[(r + j) * width + c + k] = roompts.size();
                    }
                }
            }
    overlap:;
        }
        // fill with corridors
        // pick a random spot
        while (true) {
            std::uniform_int_distribution<unsigned int> rpos(0, height / 2 - 1),
                                               cpos(0, width / 2 - 1);
            unsigned int r = rpos(mt) * 2 + 1, c = cpos(mt) * 2 + 1;
            if (!grid[r * width + c]) {
                grid[r * width + c] = -1;
                break;
            }
        }
        // hunt and kill
        while (true) {
            unsigned int cr = 0, cc = 0;
            unsigned int er = 0; ec = 0;
            // find an empty cell with a visited neighbor
            for (unsigned int r = 1; r < height - 1u; r += 2) {
                for (unsigned int c = 1; c < width - 1u; c += 2) {
                    if (!grid[r * width + c]) {
                        er = r;
                        ec = c;
                        if (c > 1 && grid[r * width + c - 2] == -1)
                            grid[r * width + c - 1] = -1;
                        else if (r > 1 && grid[(r - 2) * width + c] == -1)
                            grid[(r - 1) * width + c] = -1;
                        else if (c < width - 2 && grid[r * width + c + 2] == -1)
                            grid[r * width + c + 1] = -1;
                        else if (r < height - 2 && grid[(r + 2) * width + c] == -1)
                            grid[(r + 1) * width + c] = -1;
                        else continue;
                        cr = r;
                        cc = c;
                        goto found;
                    }
                }
            }
    found:;
            if (er == 0) break; // we're done; maze is full
            if (cr == 0) { // we've floodfilled the section we started in
                grid[er * width + c] = -1;
                continue;
            }
            while (true) {
                // clear ourselves
                grid[cr * width + cc] = -1;
                // advance in random direction
                std::uniform_int_distribution<unsigned int> compass(0, 3),
                                                            turn(0, 5);
                unsigned int sdir = compass(mt);
                // turn directions: 0 (same), 1 (right), 3 (left)
                unsigned int dirs[3];
                switch (turn(mt)) {
                case 0:
                    dirs[0] = (0 + sdir) & 3; dirs[1] = (1 + sdir) & 3; dirs[2] = (3 + sdir) & 3; break;
                case 1:
                    dirs[0] = (0 + sdir) & 3; dirs[1] = (3 + sdir) & 3; dirs[2] = (1 + sdir) & 3; break;
                case 2:
                    dirs[0] = (1 + sdir) & 3; dirs[1] = (0 + sdir) & 3; dirs[2] = (3 + sdir) & 3; break;
                case 3:
                    dirs[0] = (1 + sdir) & 3; dirs[1] = (3 + sdir) & 3; dirs[2] = (0 + sdir) & 3; break;
                case 4:
                    dirs[0] = (3 + sdir) & 3; dirs[1] = (0 + sdir) & 3; dirs[2] = (1 + sdir) & 3; break;
                case 5:
                    dirs[0] = (3 + sdir) & 3; dirs[1] = (1 + sdir) & 3; dirs[2] = (0 + sdir) & 3; break;
                }
                bool found = false;
                for (unsigned int i = 0; i < 3; i++) {
                    int dir = dirs[i];
                    switch (dir) {
                    case 0: // north
                        if (cr > 1 && !grid[(cr - 2) * width + cc]) {
                            grid[(cr - 1) * width + cc] = -1;
                            cr -= 2;
                            found = true;
                        }
                        break;
                    case 1: // east
                        if (cc < width - 2 && !grid[cr * width + cc + 2]) {
                            grid[cr * width + cc + 1] = -1;
                            cc += 2;
                            found = true;
                        }
                        break;
                    case 2: // south
                        if (cr < height - 2 && !grid[(cr + 2) * width + cc]) {
                            grid[(cr + 1) * width + cc] = -1;
                            cr += 2;
                            found = true;
                        }
                        break;
                    case 3: // west
                        if (cc > 1 && !grid[cr * width + cc - 2]) {
                            grid[cr * width + cc - 1] = -1;
                            cc -= 2;
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
        std::vector<Point> connectors;
        for (unsigned int r = 0; r < height; r++) {
            for (unsigned int c = 0; c < width; c++) {
                // check if it connects two rooms together
                if (grid[r * width + c]) continue;
                Point pt = {r, c};
                if (!useless(pt))
                    connectors.push_back(pt);
            }
        }
        std::shuffle(connectors.begin(), connectors.end(), mt);
        // merge the first room
        {
            Point p = roompts[0];
            floodfill(p);
        }
        // activate connectors
        std::bernoulli_distribution door_dist(extra_door_p);
        while (!connectors.empty()) {
            auto conn_ = std::find_if(connectors.begin(), connectors.end(), [this](const Point &p) {return touches_merged(p);});
            if (conn_ == connectors.end()) {
                // lol; some rooms were not connected. Trying again probably works :D
                break;
            }
            auto conn = *conn_;
            connectors.erase(conn_); // probably unnecessary
            grid[conn.r * width + conn.c] = -2;
            floodfill(conn);
            connectors.erase(std::remove_if(connectors.begin(), connectors.end(), [this, &door_dist](const Point &p) {
                if (!useless(p)) return false;
                if (door_dist(mt) && !touches_cleared(p)) {
                    grid[p.r * width + p.c] = -2;
                    merged[p.r * width + p.c] = true;
                }
                return true;
            }), connectors.end());
        }
        // remove some dead-ends
        std::bernoulli_distribution dead_end_dist(dead_end_p);
        bool changed = false;
        do {
            for (unsigned int r = 0; r < height; r++) {
                for (unsigned int c = 0; c < width; c++) {
                    if (grid[r * width + c] != -1) continue;
                    unsigned int count = 0;
                    if (c > 1 && grid[r * width + c - 1]) {
                        count++;
                    }
                    if (r > 1 && grid[(r - 1) * width + c]) {
                        count++;
                    }
                    if (c < width - 1u && grid[r * width + c + 1]) {
                        count++;
                    }
                    if (r < height - 1u && grid[(r + 1) * width + c]) {
                        count++;
                    }
                    if (count <= 1) {
                        if (dead_end_dist(mt)) {
                            grid[r * width + c] = -3;
                        } else {
                            grid[r * width + c] = 0;
                            changed = true;
                        }
                    }
                }
            }
        } while (changed);
    }
}

// remove
int main(int argc, char **argv) {
    generation::Dungeon dun(301, 201);
    dun.generate(100, 500);
    dun.write("maze.txt");
}
