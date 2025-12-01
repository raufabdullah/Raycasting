#ifndef GAME_H
#define GAME_H
#include "Player.h"
#include "Map.h"
#include "RaycastEngine.h"
#include "Enemy.h"
#include "Seeker.h"

class Game {
public:
    sf::RenderWindow window;
    Player player;
    Map map;
    RaycastEngine raycast;
    
    Game();
    ~Game();
    void close() { window.close(); }
    void run();
private:
    void initMap();
};

#endif