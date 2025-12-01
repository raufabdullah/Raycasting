#include "Seeker.h"
#include <cmath>

Seeker::Seeker() : alert(false), inRange(false) {
    health = 50;
    range = 10;
    damage = 10;
    speed = 3;
    count = 1;
    size = 1.0f;  
}

void Seeker::update(Player& player, const Map& map, float deltaTime) {
    sf::Vector2f diff = player.position - position;
    float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);

    // Alerts the seeker if user is in range
    updateRange(distance < range);
    if (inRange) {
        updateAlert(true);
    }

    if (alert) {
        if (distance > 0) {
            sf::Vector2f dirToPlayer = diff / distance;
            float moveSpeed = static_cast<float>(speed) * deltaTime;
            sf::Vector2f moveDelta = dirToPlayer * moveSpeed;

            // Separate X and Y for wall sliding (fixed clipping)
            sf::Vector2f newPosX = position;
            newPosX.x += moveDelta.x;
            if (!collides(newPosX, map)) {
                position.x = newPosX.x;
            }

            sf::Vector2f newPosY = position;
            newPosY.y += moveDelta.y;
            if (!collides(newPosY, map)) {
                position.y = newPosY.y;
            }
        }

        if (distance < 1.0f) {
            attack(player);
        }
    }
}