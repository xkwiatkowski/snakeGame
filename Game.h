#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <deque>
#include <vector>

enum GameState { MENU, PLAYING, GAME_OVER };

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

    GameState gameState = MENU;
    sf::Clock startAnimationClock;
    sf::Clock startDelayClock;
    bool startDelayActive = false;
    int countdownDots = 0;

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

    sf::RectangleShape appleShape;
    std::vector<Apple> apples;
    const int MAX_APPLES = 3;
    const float APPLE_LIFESPAN = 8.0f;

    sf::Clock fastEffectClock;
    float fastModeUntil = 0.f;

    std::vector<sf::Vector2i> obstacles;
    int nextObstacleScore = 5;
    void addRandomObstacle();
    void spawnInitialObstacles();

    sf::Clock deathPauseClock;
    bool deathPauseActive = false;
    const float DEATH_PAUSE_DURATION = 2.0f;

    std::vector<sf::Vector2i> disintegratingSegments;
    sf::Clock disintegrationClock;
    const float SEGMENT_DISINTEGRATION_DELAY = 0.1f;

    int applesNormal = 0;
    int applesGold = 0;
    int applesFast = 0;
};

#endif