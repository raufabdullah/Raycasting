#ifndef ENEMY_H
#define ENEMY_H
#include "Entity.h"
#include "Player.h"

class Enemy : public Entity {
private:
  int range;
  int damage;
  int speed;

protected:
  Enemy();
  Enemy(int hp);
  inline static int count = 0;

public:
  // Getters for hidden properties
  int getRange() const { return range; }
  int getDamage() const { return damage; }
  int getSpeed() const { return speed; }
  int getCount() const { return count; }

  // Setters for hidden properties
  void setRange(int r) { range = r; }
  void setDamage(int d) { damage = d; }
  void setSpeed(int s) { speed = s; }

  void attack(Player &player, int dmg = 10);
  std::string getType() const override { return "Enemy"; }

  bool collides(const sf::Vector2f &testPos, const Map &map) const override;
};

#endif
