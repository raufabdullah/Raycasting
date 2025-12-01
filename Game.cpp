#include "Game.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Constants.h"

Game::Game() : window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Raycasting Game"),
               map(MAP_WIDTH, MAP_HEIGHT),
               raycast(SCREEN_WIDTH, SCREEN_HEIGHT) {
    initMap();
    Seeker* seeker1 = new Seeker();
    seeker1->position = sf::Vector2f(8.0f, 8.0f);
    raycast.addEntity(seeker1);

    Seeker* seeker2 = new Seeker();
    seeker2->position = sf::Vector2f(15.0f, 10.0f);
    raycast.addEntity(seeker2);
}

Game::~Game() {
    for (auto entity: raycast.entities) {
        delete entity;
    }
}

void Game::initMap() {
    map.grid = map.getGrid();
}

void Game::run() {
    sf::Clock clock;
    window.setFramerateLimit(60);
    while (window.isOpen() && raycast.getIsRunning()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) close();
        }
        float deltaTime = clock.restart().asSeconds();
        player.move(deltaTime, map, sf::Keyboard::W, sf::Keyboard::S);
        player.rotate(deltaTime, sf::Keyboard::A, sf::Keyboard::D);

        for (auto entity : raycast.entities) {
            entity->update(player, map, deltaTime);
        }

        window.clear(sf::Color::Black);
        raycast.render(window, player, map);
        window.display();
    }
}