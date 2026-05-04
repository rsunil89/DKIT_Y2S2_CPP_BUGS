#ifndef HOPPER_H
#define HOPPER_H

#include "Bug.h"

class Hopper : public Bug {
private:
    int hopLength; // Distance of a hop

public:
    Hopper(int id, pair<int, int> pos, Direction dir, int health, int hopLength);

    string getType() const override { return "Hopper"; }
    int getHopLength() const override;

    void move() override;
};

#endif
