#ifndef SEEKER_H
#define SEEKER_H
#include "Enemy.h"

class Seeker : public Enemy {
public:
    bool alert;
    bool inRange;

    Seeker();
    void updateAlert(bool newAlert) { alert = newAlert; }
    void updateRange(bool newInRange) { inRange = newInRange; }

    void update(Player& player, const Map& map, float deltaTime) override;
    std::string getType() const override { return "Seeker"; }
};

#endif