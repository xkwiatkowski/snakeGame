#include "Game.h"

Game::Game() : window(sf::VideoMode(600, 600), "Snake Game") {
    snakePosition = {GRID_SIZE / 2, GRID_SIZE / 2};
    snakeBody.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    snakeBody.setFillColor(sf::Color::Green);
}

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
    
    drawGrid();
    drawSnake();
    
    window.display();
}

void Game::drawGrid(){
    sf::RectangleShape line(sf::Vector2f(WINDOW_SIZE, 1));
    line.setFillColor(sf::Color(50, 50, 50));

    for (int i=0; i<=GRID_SIZE; ++i) {
        line.setPosition(0, i * CELL_SIZE);
        window.draw(line);
    }

    line.setSize(sf::Vector2f(1, WINDOW_SIZE));
    for (int i=0; i<=GRID_SIZE; ++i) {
        line.setPosition(i * CELL_SIZE, 0);
        window.draw(line);
    }
}

void Game::drawSnake(){
    snakeBody.setPosition(snakePosition.x * CELL_SIZE, snakePosition.y * CELL_SIZE);
    window.draw(snakeBody);
}