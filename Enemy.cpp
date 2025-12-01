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