#ifndef ENEMY_H
#define ENEMY_H
#include "Entity.h"
#include "Player.h"

class Enemy : public Entity {
protected:
    int range;
    int damage;
    int speed;
    inline static int count = 0;

    Enemy();
    Enemy(int hp);
    void attack(Player& player, int dmg = 10);
    int getCount() const { return count; }

    std::string getType() const override { return "Enemy"; }

    bool collides(const sf::Vector2f& testPos, const Map& map) const override;
};

#endif