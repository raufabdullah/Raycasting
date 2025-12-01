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
    sf::Vector2f moveDir = dir * moveSpeed;

    if (sf::Keyboard::isKeyPressed(forward)) {
        sf::Vector2f newPos = position + moveDir;
        if (map.grid[static_cast<int>(newPos.y)][static_cast<int>(newPos.x)] == 0) position = newPos;
    }
    if (sf::Keyboard::isKeyPressed(backward)) {
        sf::Vector2f newPos = position - moveDir;
        if (map.grid[static_cast<int>(newPos.y)][static_cast<int>(newPos.x)] == 0) position = newPos;
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