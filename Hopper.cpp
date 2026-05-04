#include "Hopper.h"
#include <cstdlib>

Hopper::Hopper(int id, pair<int, int> pos, Direction dir, int health, int hopLength)
    : Bug(id, pos, dir, health), hopLength(hopLength) {}

int Hopper::getHopLength() const {
    return hopLength;
}

void Hopper::move() {
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

    // Hop by hopLength cells in the current direction
    int x = position.first;
    int y = position.second;

    switch (direction) {
        case Direction::NORTH:
            y = (y - hopLength) % 10;
            if (y < 0) y += 10;
            break;
        case Direction::EAST:
            x = (x + hopLength) % 10;
            break;
        case Direction::SOUTH:
            y = (y + hopLength) % 10;
            break;
        case Direction::WEST:
            x = (x - hopLength) % 10;
            if (x < 0) x += 10;
            break;
        default:
            break;
    }

    setPosition({x, y});
}
