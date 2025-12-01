#ifndef RAYCASTENGINE_H
#define RAYCASTENGINE_H
#include <algorithm>
#include "Entity.h" 
#include "Player.h"
#include "Map.h"
#include <vector>
#include <SFML/Graphics.hpp>

class Game; 

class RaycastEngine {
public:
    int screenWidth;
    int screenHeight;
    bool isRunning;
    int renderDist;

    std::vector<Entity*> entities;

    struct SpriteInfo {
        size_t index;
        double dist;
        bool operator<(const SpriteInfo& other) const { return dist > other.dist; }
    };

    RaycastEngine(int w, int h);
    void addEntity(Entity* entity);
    bool getIsRunning() const { return isRunning; }
    void updateRun(bool run) { isRunning = run; }

    void render(sf::RenderWindow& window, const Player& player, const Map& map);
};

#endif