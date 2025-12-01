#include "Enemy.h"

Enemy::Enemy() : range(5), damage(10), speed(3){
    count++;
    health = 50;
}

Enemy::Enemy(int hp) : range(5), damage(10), speed(3){
    count++;
    health = hp;
}

void Enemy::attack(Player& player, int dmg) {
    player.damage(dmg);
}

bool Enemy::collides(const sf::Vector2f& testPos, const Map& map) const {
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