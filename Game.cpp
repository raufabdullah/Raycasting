#include "Constants.h"
#include "Game.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

Game::Game() :  window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Raycasting Game"),
                map(MAP_WIDTH, MAP_HEIGHT), raycast(SCREEN_WIDTH, SCREEN_HEIGHT) {
    initMap();
    // spawn two seekers, one at (8, 8) and one at (15, 10)
    Seeker *seeker1 = new Seeker();
    seeker1->setPosition(sf::Vector2f(8.0f, 8.0f));
    raycast.addEntity(seeker1);

    Seeker *seeker2 = new Seeker();
    seeker2->setPosition(sf::Vector2f(15.0f, 10.0f));
    raycast.addEntity(seeker2);
    currentState = Menu; // starts in menu state

    // load font, if it fails just return (no text)
    if (!font.loadFromFile("arial.ttf")) {
        return;
    }

    // Start Button when in Menu State
    startButton.setSize(sf::Vector2f(200, 50));
    startButton.setFillColor(sf::Color::Green); // Green
    startButton.setOrigin(100, 25);  // center origin
    startButton.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    startText.setFont(font);
    startText.setString("START");
    startText.setCharacterSize(24);
    startText.setFillColor(sf::Color::Black);
    // center the text on the button
    sf::FloatRect textRect = startText.getLocalBounds();
    startText.setOrigin(textRect.left + textRect.width / 2.0f,
                        textRect.top + textRect.height / 2.0f);
    startText.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

    // Death screen text (big red text that says "You got caught")
    deathText.setFont(font);
    deathText.setString("You got caught");
    deathText.setCharacterSize(48);
    deathText.setFillColor(sf::Color::Red);
    // center it
    sf::FloatRect deathRect = deathText.getLocalBounds();
    deathText.setOrigin(deathRect.left + deathRect.width / 2.0f,
                        deathRect.top + deathRect.height / 2.0f);
    deathText.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 50);

    // survival time text (shows how long you lasted)
    survivalText.setFont(font);
    survivalText.setCharacterSize(32);
    survivalText.setFillColor(sf::Color::White);
    survivalTime = 0.0f;
}

Game::~Game() {
    // cleanup all entities (prevent memory leaks)
    for (auto entity : raycast.entities) {
        delete entity;
    }
}

void Game::initMap() { map.grid = map.getGrid(); }

void Game::run() {
    sf::Clock clock;
    window.setFramerateLimit(60);  // cap at 60 fps
    while (window.isOpen() && raycast.getIsRunning()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                close();
        }

        // handle button click only when in menu
        if (currentState == Menu && event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                // Check if mouse click happens inside the button
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                if (startButton.getGlobalBounds().contains(mousePos)) {
                    // set position and health whenever we start the game to a fixed position
                    player.sethealth(100);
                    player.setPosition(sf::Vector2f(22.0f, 12.0f));
                    
                    // Reset all enemies to starting positions (so they don't spawn right next to you)
                    for (auto entity : raycast.entities) {
                        if (entity->getType() == "Seeker") {
                            // Reset to original positions based on entity index
                            if (entity == raycast.entities[0]) {
                                entity->setPosition(sf::Vector2f(8.0f, 8.0f));
                            } else if (entity == raycast.entities[1]) {
                                entity->setPosition(sf::Vector2f(15.0f, 10.0f));
                            }
                        }
                    }
                    
                    gameTimer.restart();  // start counting survival time
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
            if (player.gethealth() <= 0) {
                // dead, calculate survival time
                survivalTime = gameTimer.getElapsedTime().asSeconds();
                survivalText.setString("You survived for: " + std::to_string(static_cast<int>(survivalTime)) + " second(s)");

                // center the survival text
                sf::FloatRect survivalRect = survivalText.getLocalBounds();
                survivalText.setOrigin(survivalRect.left + survivalRect.width / 2.0f,
                                       survivalRect.top + survivalRect.height / 2.0f);
                survivalText.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 50);

                deathTimer.restart();  // start the 2 second death screen timer
                currentState = DeathScreen;
            }

            // movement
            player.move(deltaTime, map, sf::Keyboard::W, sf::Keyboard::S);
            player.rotate(deltaTime, sf::Keyboard::A, sf::Keyboard::D);

            // update all entities (seekers move toward player)
            for (auto entity : raycast.entities) {
                entity->update(player, map, deltaTime);
            }

            // Render
            raycast.render(window, player, map);
        } else if (currentState == DeathScreen) {
          
            // Show death screen for 2 seconds then return to menu (so you can try again)
            window.draw(deathText);
            window.draw(survivalText);
            
            if (deathTimer.getElapsedTime().asSeconds() >= 2.0f) {
                currentState = Menu;  // back to menu after 2 seconds
            }
        }
        window.display();
    }
}