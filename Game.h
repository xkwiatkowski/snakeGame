#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <deque>

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update();
    void render();

    sf::RenderWindow window;

    const int WINDOW_SIZE = 600;
    const int GRID_SIZE = 20;
    const int CELL_SIZE = WINDOW_SIZE / GRID_SIZE;

    void drawGrid();
    void drawSnake();

    std::deque<sf::Vector2i> snake;
    int snakeLenght = 1;

    sf::RectangleShape snakeBody;
    sf::Vector2i direction;
    sf::Clock movementClock;
    float moveDelay = 0.15f;

    void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);

    sf::Vector2i applePosition;
    sf::RectangleShape appleShape;

    void spawnApple();
    void drawApple();

    const float TIME_PER_FRAME = 1.f / 10.f;

    bool gameStarted = false;
    bool gameOver = false;

    void resetGame();
};

#endif