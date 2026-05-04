#include "Crawler.h"
#include <cstdlib>
#include <ctime>

Crawler::Crawler(int id, pair<int, int> pos, Direction dir, int health)
    : Bug(id, pos, dir, health) {}

void Crawler::move() {
    if (!alive) return;

    // If way is blocked, set a new random direction (repeat until bug can move forward)
    while (isWayBlocked()) {
        int randDir = rand() % 4 + 1;  // Random number between 1 and 4
        direction = static_cast<Direction>(randDir);
    }

    // Move one cell in the current direction
    int x = position.first;
    int y = position.second;

    switch (direction) {
        case Direction::NORTH:
            y--;
            break;
        case Direction::EAST:
            x++;
            break;
        case Direction::SOUTH:
            y++;
            break;
        case Direction::WEST:
            x--;
            break;
        default:
            break;
    }

    // Record new position in the crawler's path history
    setPosition({x, y});
}
