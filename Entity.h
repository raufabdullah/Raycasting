#ifndef ENTITY_H
#define ENTITY_H
#include "Constants.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

class Entity {
private:
  sf::Vector2f position;
  sf::Vector2f velocity;
  int health;
  bool isActive;
  float size;

public:
  virtual std::string getType() const { return "Entity"; }

  Entity()
      : position(0, 0), velocity(0, 0), health(100), isActive(true),
        size(1.0f) {}

  // Health
  void damage(int amt) {
    health -= amt;
    if (health <= 0)
      isActive = false;
  }
  int getHealth() const { return health; }
  void setHealth(int h) { health = h; }

  // Position
  void setPosition(sf::Vector2f pos) { position = pos; }
  sf::Vector2f getPosition() const { return position; }

  // Velocity
  void setVelocity(sf::Vector2f vel) { velocity = vel; }
  sf::Vector2f getVelocity() const { return velocity; }

  // Size
  void setSize(float s) { size = s; }
  float getSize() const { return size; }

  // Status
  bool getIsActive() const { return isActive; }
  void setIsActive(bool active) { isActive = active; }

  virtual void update(class Player &player, const class Map &map,
                      float deltaTime) {}
  virtual void render(sf::RenderWindow &window) {}

  virtual bool collides(const sf::Vector2f &testPos, const Map &map) const = 0;
};

#endif
