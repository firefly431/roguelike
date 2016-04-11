#include <generation/dungeon.h>

int main(int argc, char **argv) {
    generation::Dungeon dun(301, 201);
    dun.generate(100, 500);
    dun.write("maze.txt");
}
