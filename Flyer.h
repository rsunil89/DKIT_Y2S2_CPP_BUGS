#ifndef FLYER_H
#define FLYER_H

#include "Bug.h"

class Flyer : public Bug {
public:
    Flyer(int id, pair<int, int> pos, Direction dir, int health);

    string getType() const override { return "Flyer"; }
    void move() override;
};

#endif
