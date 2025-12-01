#include "Constants.h"
#include "Game.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

Game::Game()
    : window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Raycasting Game"),
      map(MAP_WIDTH, MAP_HEIGHT), raycast(SCREEN_WIDTH, SCREEN_HEIGHT) {
  initMap();
  Seeker *seeker1 = new Seeker();
  seeker1->position = sf::Vector2f(8.0f, 8.0f);
  raycast.addEntity(seeker1);

  Seeker *seeker2 = new Seeker();
  seeker2->position = sf::Vector2f(15.0f, 10.0f);
  raycast.addEntity(seeker2);
  currentState = Menu; // starts in menu state

  if (!font.loadFromFile("arial.ttf")) {
    return;
  }

  // Start Button when in Menu State
  startButton.setSize(sf::Vector2f(200, 50));
  startButton.setFillColor(sf::Color::Green); // Green
  startButton.setOrigin(100, 25);
  startButton.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
  startText.setFont(font);
  startText.setString("START");
  startText.setCharacterSize(24);
  startText.setFillColor(sf::Color::Black);
  sf::FloatRect textRect = startText.getLocalBounds();
  startText.setOrigin(textRect.left + textRect.width / 2.0f,
                      textRect.top + textRect.height / 2.0f);
  startText.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
}

Game::~Game() {
  for (auto entity : raycast.entities) {
    delete entity;
  }
}

void Game::initMap() { map.grid = map.getGrid(); }

void Game::run() {
  sf::Clock clock;
  window.setFramerateLimit(60);
  while (window.isOpen() && raycast.getIsRunning()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        close();
    }
    if (currentState == Menu && event.type == sf::Event::MouseButtonPressed) {
      if (event.mouseButton.button == sf::Mouse::Left) {
        // Check if mouse click happens inside the button
        sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
        if (startButton.getGlobalBounds().contains(mousePos)) {

          // set position and health whenever we start the game to a fixed
          // position
          player.health = 100;
          player.position = sf::Vector2f(22.0f, 12.0f);
          
          // Reset all enemies to starting positions
          for (auto entity : raycast.entities) {
            if (entity->getType() == "Seeker") {
              // Reset to original positions based on entity index
              if (entity == raycast.entities[0]) {
                entity->position = sf::Vector2f(8.0f, 8.0f);
              } else if (entity == raycast.entities[1]) {
                entity->position = sf::Vector2f(15.0f, 10.0f);
              }
            }
          }
          
          currentState = Playing;
        }
      }
    }
    float deltaTime = clock.restart().asSeconds();
    window.clear(sf::Color::Black);
    if (currentState == Menu) {
      // Draw button (green, white text)
      window.draw(startButton);
      window.draw(startText);
    } else if (currentState == Playing) {
      // game logic if current state is playing
      if (player.health <= 0) {
        currentState = Menu;
      }
      player.move(deltaTime, map, sf::Keyboard::W, sf::Keyboard::S);
      player.rotate(deltaTime, sf::Keyboard::A, sf::Keyboard::D);

      for (auto entity : raycast.entities) {
        entity->update(player, map, deltaTime);
      }

      // Render
      raycast.render(window, player, map);
    }
    window.display();
  }
}