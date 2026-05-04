#include "Crawler.h"
#include <cstdlib>

Crawler::Crawler(int id, pair<int, int> pos, Direction dir, int health)
    : Bug(id, pos, dir, health) {}

void Crawler::move() {
    if (!alive) return;

    // If way is blocked, turn 90 degrees clockwise
    if (isWayBlocked()) {
        switch (direction) {
            case Direction::NORTH: direction = Direction::EAST;  break;
            case Direction::EAST:  direction = Direction::SOUTH; break;
            case Direction::SOUTH: direction = Direction::WEST;  break;
            case Direction::WEST:  direction = Direction::NORTH; break;
            default: break;
        }
    }

    // Move one cell in the current direction
    int x = position.first;
    int y = position.second;

    switch (direction) {
        case Direction::NORTH:
            if (y > 0) y--;
            break;
        case Direction::EAST:
            if (x < 9) x++;
            break;
        case Direction::SOUTH:
            if (y < 9) y++;
            break;
        case Direction::WEST:
            if (x > 0) x--;
            break;
        default:
            break;
    }

    setPosition({x, y});
}
