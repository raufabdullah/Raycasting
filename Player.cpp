#include "Player.h"
#include <cmath>
#include "Constants.h"
#include "Map.h"
#include <vector>
#include <cmath>

Player::Player() : speed(MOVE_SPEED), dir(-1.0f, 0.0f), plane(0.0f, 0.66f) {
    position = sf::Vector2f(22.0f, 12.0f);  // Starting pos
}

void Player::damage(int dmg) {
    health -= dmg;
}

void Player::move(float deltaTime, const Map& map, const sf::Keyboard::Key forward, const sf::Keyboard::Key backward) {
    float moveSpeed = speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(forward)) {
        sf::Vector2f moveDelta = dir * moveSpeed;

        // Separate X and Y for wall sliding (prevents clipping)
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

    if (sf::Keyboard::isKeyPressed(backward)) {
        sf::Vector2f moveDelta = dir * (-moveSpeed);

        // Separate X and Y for wall sliding (no clipping ee)
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
}

void Player::rotate(float deltaTime, const sf::Keyboard::Key left, const sf::Keyboard::Key right) {
    float rotSpeed = ROT_SPEED * deltaTime;

    if (sf::Keyboard::isKeyPressed(right)) rotSpeed = -rotSpeed;  // Invert for right turn
    if (sf::Keyboard::isKeyPressed(left) || sf::Keyboard::isKeyPressed(right)) {
        float oldDirX = dir.x;
        dir.x = dir.x * cos(rotSpeed) - dir.y * sin(rotSpeed);
        dir.y = oldDirX * sin(rotSpeed) + dir.y * cos(rotSpeed);

        float oldPlaneX = plane.x;
        plane.x = plane.x * cos(rotSpeed) - plane.y * sin(rotSpeed);
        plane.y = oldPlaneX * sin(rotSpeed) + plane.y * cos(rotSpeed);
    }
}

bool Player::collides(const sf::Vector2f& testPos, const Map& map) const {
    const float radius = 0.3f;

    // Check 5 points: 4 corners of bounding box + center
    std::vector<sf::Vector2f> points = {
        {testPos.x - radius, testPos.y - radius},  // Bottom-left
        {testPos.x + radius, testPos.y - radius},  // Bottom-right
        {testPos.x - radius, testPos.y + radius},  // Top-left
        {testPos.x + radius, testPos.y + radius},  // Top-right
        testPos  // Center
    };

    for (const auto& p : points) {
        int mx = static_cast<int>(p.x);
        int my = static_cast<int>(p.y);
        if (mx < 0 || mx >= map.mapWidth || my < 0 || my >= map.mapHeight ||
            map.grid[my][mx] != 0) {
            return true;  // Hits wall or out of bounds
        }
    }
    return false;  // Safe
}