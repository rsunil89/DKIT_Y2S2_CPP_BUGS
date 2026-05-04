#ifndef CRAWLER_H
#define CRAWLER_H

#include "Bug.h"

class Crawler : public Bug {
public:
    Crawler(int id, pair<int, int> pos, Direction dir, int health);

    string getType() const override { return "Crawler"; }
    void move() override;
};

#endif
