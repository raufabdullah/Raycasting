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

    updateRange(distance < range);
    if (inRange) {
        updateAlert(true);
    }

    if (alert) {
        if (distance > 0) {
            sf::Vector2f dirToPlayer = diff / distance;
            sf::Vector2f move = dirToPlayer * static_cast<float>(speed) * deltaTime;
            sf::Vector2f newPos = position + move;

            int mapX = static_cast<int>(newPos.x);
            int mapY = static_cast<int>(newPos.y);
            if (mapX >= 0 && mapX < map.mapWidth && mapY >= 0 && mapY < map.mapHeight &&
                map.grid[mapY][mapX] == 0) {
                position = newPos;
            }
        }

        if (distance < 1.0f) {
            attack(player);
        }
    }
}