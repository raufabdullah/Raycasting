#ifndef ENTITY_H
#define ENTITY_H
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath> 
#include "Constants.h"

class Entity {
public:
    sf::Vector2f position;
    sf::Vector2f velocity;
    int health;
    bool isActive;
    float size = 1.0f; 
    virtual std::string getType() const { return "Entity"; }

    Entity() : position(0, 0), velocity(0, 0), health(100), isActive(true) {}
    void damage(int amt) { health -= amt; if (health <= 0) isActive = false; }
    virtual void update(class Player& player, const class Map& map, float deltaTime) {}
    virtual void render(sf::RenderWindow& window) {}

    virtual bool collides(const sf::Vector2f& testPos, const Map& map) const = 0;
};

#endif