#include "Bug.h"
#include <sstream>

Bug::Bug(int id, pair<int, int> pos, Direction dir, int health)
    : id(id), position(pos), direction(dir), health(health), alive(true), eatenBy(-1) {
    path.push_back(pos);
}

int Bug::getId() const { return id; }

pair<int, int> Bug::getPosition() const { return position; }

Direction Bug::getDirection() const { return direction; }

int Bug::getHealth() const { return health; }

bool Bug::isAlive() const { return alive; }

int Bug::getEatenBy() const { return eatenBy; }

const list<pair<int, int>>& Bug::getPath() const { return path; }

void Bug::setPosition(pair<int, int> pos) {
    position = pos;
    path.push_back(pos);
}

void Bug::setDirection(Direction dir) {
    direction = dir;
}

void Bug::kill() {
    alive = false;
}

void Bug::setEatenBy(int bugId) {
    eatenBy = bugId;
}

void Bug::takeDamage(int amount) {
    health = max(0, health - amount);
    if (health == 0) {
        alive = false;
    }
}

bool Bug::isWayBlocked() const {
    int x = position.first;
    int y = position.second;

    switch (direction) {
        case Direction::NORTH:
            return (y == 0);
        case Direction::EAST:
            return (x == 9);
        case Direction::SOUTH:
            return (y == 9);
        case Direction::WEST:
            return (x == 0);
        default:
            return false;
    }
}

string Bug::toString() const {
    string dirStr;
    switch (direction) {
        case Direction::NORTH: dirStr = "North"; break;
        case Direction::EAST:  dirStr = "East";  break;
        case Direction::SOUTH: dirStr = "South"; break;
        case Direction::WEST:  dirStr = "West";  break;
        default:               dirStr = "None";  break;
    }

    return "ID: " + to_string(id)
        + " | Position: (" + to_string(position.first) + "," + to_string(position.second) + ")"
        + " | Direction: " + dirStr
        + " | Health: " + to_string(health)
        + " | Status: " + (alive ? "Alive" : "Dead");
}

string Bug::lifeHistoryToString() const {
    string history = to_string(id) + " " + getType() + " Path: ";
    bool first = true;
    for (const auto& p : path) {
        if (!first) history += ",";
        history += "(" + to_string(p.first) + "," + to_string(p.second) + ")";
        first = false;
    }
    if (!alive && eatenBy != -1) {
        history += " Eaten by " + to_string(eatenBy);
    } else if (alive) {
        history += " Alive!";
    } else {
        history += " Dead";
    }
    return history;
}
