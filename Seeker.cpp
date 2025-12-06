#include "Seeker.h"
#include <cmath>

Seeker::Seeker() : alert(false), inRange(false) {
  setHealth(50);
  setRange(10);
  setDamage(10);
  setSpeed(3);
  setSize(1.0f);
  Enemy::count = 1;
}

void Seeker::update(Player &player, const Map &map, float deltaTime) {
  sf::Vector2f playerPos = player.getPosition();
  sf::Vector2f myPos = getPosition();
  sf::Vector2f diff = playerPos - myPos;
  float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);

  // Alerts the seeker if user is in range
  updateRange(distance < getRange());
  if (inRange) {
    updateAlert(true);
  }

  if (alert) {
    if (distance > 0) {
      sf::Vector2f dirToPlayer = diff / distance;
      float moveSpeed = static_cast<float>(getSpeed()) * deltaTime;
      sf::Vector2f moveDelta = dirToPlayer * moveSpeed;
      sf::Vector2f pos = getPosition();

      // Separate X and Y for wall sliding (fixed clipping)
      sf::Vector2f newPosX = pos;
      newPosX.x += moveDelta.x;
      if (!collides(newPosX, map)) {
        pos.x = newPosX.x;
      }

      sf::Vector2f newPosY = pos;
      newPosY.y += moveDelta.y;
      if (!collides(newPosY, map)) {
        pos.y = newPosY.y;
      }

      setPosition(pos);
    }

    if (distance < 1.0f) {
      attack(player);
    }
  }
}
