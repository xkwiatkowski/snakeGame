#include "Game.h"

Game::Game() : window(sf::VideoMode(600, 600), "Snake Game") {}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
            window.close();
        }
    }
}

void Game::update() {
    // logika gry
}

void Game::render() {
    window.clear(sf::Color::Black);
    
    // rysowanie elementow gry
    
    window.display();
}