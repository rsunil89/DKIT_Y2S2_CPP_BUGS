#ifndef BUG_H
#define BUG_H

#include <iostream>
#include <utility>
#include <list>
#include <string>
#include "Direction.h"

using namespace std;

class Bug {
protected:
    int id;
    pair<int, int> position;   // (x,y) where (0,0) is top-left
    Direction direction;
    int health;                // 1-20, biggest wins in a fight
    bool alive;
    list<pair<int, int>> path; // List of positions visited

public:
    Bug(int id, pair<int, int> pos, Direction dir, int health);
    virtual ~Bug() {}

    // Getters
    int getId() const;
    pair<int, int> getPosition() const;
    Direction getDirection() const;
    int getHealth() const;
    bool isAlive() const;
    const list<pair<int, int>>& getPath() const;

    // Setters
    void setPosition(pair<int, int> pos);
    void setDirection(Direction dir);
    void kill();

    // Virtual - each bug type returns its type name
    virtual string getType() const = 0;

    // Virtual - returns hop length (0 for non-hoppers)
    virtual int getHopLength() const { return 0; }

    // Pure virtual - each bug type implements its own movement
    virtual void move() = 0;

    // Checks if bug is against an edge AND facing that edge direction
    bool isWayBlocked() const;

    // Display helpers
    string toString() const;
    string lifeHistoryToString() const;
};

#endif
