#ifndef ENTITY_H
#define ENTITY_H
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath> 
#include "Constants.h"

class Entity {
protected:
    sf::Vector2f velocity;
    int health;
    bool isActive;
    
public:
    sf::Vector2f position;
    float size = 1.0f;
    
public:
    virtual std::string getType() const { return "Entity"; }

    Entity() : position(0, 0), velocity(0, 0), health(100), isActive(true) {}
    void damage(int amt) { health -= amt; if (health <= 0) isActive = false; }
    virtual void update(class Player& player, const class Map& map, float deltaTime) {}
    virtual void render(sf::RenderWindow& window) {}

    int gethealth() const { return health; } 
    void sethealth(int h) { health = h; }
    void setPosition(sf::Vector2f pos) { position = pos; }
    sf::Vector2f getposition() const { return position; }

    virtual bool collides(const sf::Vector2f& testPos, const Map& map) const = 0;
};

#endif