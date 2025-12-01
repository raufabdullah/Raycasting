#ifndef PLAYER_H
#define PLAYER_H
#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "Map.h"

class Player : public Entity {
public:
    float speed; 
    sf::Vector2f dir;   
    sf::Vector2f plane; 

    Player();
    bool updateSpeed(int newSpeed) { speed = newSpeed; return true; }

    void damage(int dmg);
    void move(float deltaTime, const Map& map, const sf::Keyboard::Key forward, const sf::Keyboard::Key backward);
    void rotate(float deltaTime, const sf::Keyboard::Key left, const sf::Keyboard::Key right);
};

#endif