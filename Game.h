#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <deque>
#include <vector>

enum AppleType { NORMAL, GOLD, FAST };

struct Apple {
    sf::Vector2i position;
    AppleType type;
    sf::Clock timer;
};

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
    void drawObstacles();
    void drawApples();
    void spawnApple();
    void spawnInitialApples();
    void resetGame();
    void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);

    std::deque<sf::Vector2i> snake;
    int snakeLenght = 1;
    sf::Vector2i direction;
    sf::RectangleShape snakeBody;

    sf::Clock movementClock;
    float moveDelay = 0.1f;
    const float DEFAULT_MOVE_DELAY = 0.1f;

    bool gameStarted = false;
    bool gameOver = false;

    int score = 0;
    sf::Font font;
    sf::Text scoreText;
    void updateScoreText();

    sf::RectangleShape appleShape;
    std::vector<Apple> apples;
    const int MAX_APPLES = 3;
    const float APPLE_LIFESPAN = 8.0f;

    sf::Clock fastEffectClock;
    float fastModeUntil = 0.f;

    std::vector<sf::Vector2i> obstacles;
    int nextObstacleScore = 5;
    void addRandomObstacle();
};

#endif