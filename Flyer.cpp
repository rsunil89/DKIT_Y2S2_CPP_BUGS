#include "Flyer.h"
#include <cstdlib>

Flyer::Flyer(int id, pair<int, int> pos, Direction dir, int health)
    : Bug(id, pos, dir, health) {}

void Flyer::move() {
    if (!alive) return;

    // Flyer moves diagonally: 1 unit in both X and Y directions
    // North: (+1, -1) North-East
    // East:  (+1, +1) South-East
    // South: (-1, +1) South-West
    // West:  (-1, -1) North-West
    //
    // If way is blocked, try a new random direction

    int attempts = 0;
    const int MAX_ATTEMPTS = 20;

    do {
        int x = position.first;
        int y = position.second;
        bool blocked = false;

        switch (direction) {
            case Direction::NORTH:
                if (y == 0) blocked = true;  // Can't go up
                break;
            case Direction::EAST:
                if (x == 9) blocked = true;  // Can't go right
                break;
            case Direction::SOUTH:
                if (y == 9) blocked = true;  // Can't go down
                break;
            case Direction::WEST:
                if (x == 0) blocked = true;  // Can't go left
                break;
            default:
                break;
        }

        if (!blocked) break;

        // Try a new random direction
        int randDir = rand() % 4 + 1;
        direction = static_cast<Direction>(randDir);
        attempts++;

    } while (attempts < MAX_ATTEMPTS);

    // Move diagonally based on direction
    int x = position.first;
    int y = position.second;

    switch (direction) {
        case Direction::NORTH: {
            // North-East diagonal (+1, -1)
            int newX = x + 1;
            int newY = y - 1;
            if (newX > 9) newX = 9;
            if (newY < 0) newY = 0;
            x = newX;
            y = newY;
            break;
        }
        case Direction::EAST: {
            // South-East diagonal (+1, +1)
            int newX = x + 1;
            int newY = y + 1;
            if (newX > 9) newX = 9;
            if (newY > 9) newY = 9;
            x = newX;
            y = newY;
            break;
        }
        case Direction::SOUTH: {
            // South-West diagonal (-1, +1)
            int newX = x - 1;
            int newY = y + 1;
            if (newX < 0) newX = 0;
            if (newY > 9) newY = 9;
            x = newX;
            y = newY;
            break;
        }
        case Direction::WEST: {
            // North-West diagonal (-1, -1)
            int newX = x - 1;
            int newY = y - 1;
            if (newX < 0) newX = 0;
            if (newY < 0) newY = 0;
            x = newX;
            y = newY;
            break;
        }
        default:
            break;
    }

    // Record new position
    setPosition({x, y});
}
