#include "Hopper.h"
#include <cstdlib>
#include <ctime>

Hopper::Hopper(int id, pair<int, int> pos, Direction dir, int health, int hopLength)
    : Bug(id, pos, dir, health), hopLength(hopLength) {}

int Hopper::getHopLength() const {
    return hopLength;
}

void Hopper::move() {
    if (!alive) return;

    // If way is blocked, set a new random direction (repeat until bug can move forward)
    while (isWayBlocked()) {
        int randDir = rand() % 4 + 1;  // Random number between 1 and 4
        direction = static_cast<Direction>(randDir);
    }

    // Hop by hopLength cells in the current direction
    int x = position.first;
    int y = position.second;

    switch (direction) {
        case Direction::NORTH: {
            int newY = y - hopLength;
            if (newY < 0) newY = 0;  // Hit the edge, fall on the square where it hit
            y = newY;
            break;
        }
        case Direction::EAST: {
            int newX = x + hopLength;
            if (newX > 9) newX = 9;  // Hit the edge, fall on the square where it hit
            x = newX;
            break;
        }
        case Direction::SOUTH: {
            int newY = y + hopLength;
            if (newY > 9) newY = 9;  // Hit the edge, fall on the square where it hit
            y = newY;
            break;
        }
        case Direction::WEST: {
            int newX = x - hopLength;
            if (newX < 0) newX = 0;  // Hit the edge, fall on the square where it hit
            x = newX;
            break;
        }
        default:
            break;
    }

    // Record new position in the hopper's path history
    setPosition({x, y});
}
