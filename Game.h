#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>

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

    sf::Vector2i snakePosition;
    sf::RectangleShape snakeBody;
    sf::Vector2i direction;
    sf::Clock movementClock;
    float moveDelay = 0.15f;

    void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
};

#endif