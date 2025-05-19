#include "Game.h"
#include <cstdlib>
#include <ctime>
#include <cmath>

Game::Game() : window(sf::VideoMode(600, 600), L"Snake Game") {
    sf::Vector2i startPos = {GRID_SIZE / 2, GRID_SIZE / 2};
    for(int i=0; i<snakeLenght; ++i){
        snake.push_back({startPos.x - i, startPos.y});
    }
    snakeBody.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    snakeBody.setFillColor(sf::Color::Green);

    moveDelay = DEFAULT_MOVE_DELAY;
    direction = {1, 0};
    gameStarted = false;

    appleShape.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    appleShape.setFillColor(sf::Color::Red);
    spawnApple();
}

void Game::run() {
    sf::Clock clock;
    float accumulatedTime = 0.f;

    while (window.isOpen()) {
        processEvents();

        float deltaTime = clock.restart().asSeconds();
        accumulatedTime += deltaTime;

        while(accumulatedTime >= TIME_PER_FRAME){
            update();
            accumulatedTime -= TIME_PER_FRAME;
        }

        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
            window.close();
        } else if(event.type == sf::Event::KeyPressed){
            handlePlayerInput(event.key.code, true);
        }

        if(event.key.code == sf::Keyboard::R && gameOver){
            resetGame();
            return;
        }
    }
}

void Game::update() {
    if(!gameStarted) return;

    if(fastEffectClock.getElapsedTime().asSeconds() > fastModeUntil){
        moveDelay = DEFAULT_MOVE_DELAY;
    }

    sf::Vector2i newHead = snake.front() + direction;

    if(newHead.x < 0) newHead.x = GRID_SIZE - 1;
    else if(newHead.x >= GRID_SIZE) newHead.x = 0;
    if(newHead.y < 0) newHead.y = GRID_SIZE - 1;
    else if(newHead.y >= GRID_SIZE) newHead.y = 0;

    for(const auto& segment : snake){
        if(segment == newHead){
            gameOver = true;
            return;
        }
    }

    if(newHead == applePosition){
        snakeLenght++;
        switch(currentAppleType){
            case NORMAL:
                score += 1;
                break;
            case GOLD:
                score += 3;
                break;
            case FAST:
                score += 1;
                fastModeUntil = fastEffectClock.getElapsedTime().asSeconds() + 5.0f;
                moveDelay = 0.03f;
                break;
        }
        spawnApple();
    }

    if((currentAppleType == GOLD || currentAppleType == FAST) && appleTimer.getElapsedTime().asSeconds() > APPLE_LIFESPAN){
        spawnApple();
    }

    snake.push_front(newHead);
    if(snake.size() > snakeLenght){
        snake.pop_back();
    }
}

void Game::render() {
    bool fastActive = fastEffectClock.getElapsedTime().asSeconds() <= fastModeUntil;
    window.clear(fastActive ? sf::Color(20, 20, 40) : sf::Color::Black);
    
    drawGrid();
    drawSnake();
    drawApple();
    for(int i=0; i<score; ++i){
        sf::CircleShape dot(5.f);
        dot.setPointCount(30);
        dot.setFillColor(sf::Color(255, 220, 100));
        dot.setPosition(10 + i * 15, 10);
        window.draw(dot);
    }
    
    window.display();

    // if(gameOver){
    //     sf::Font font;
    //     if(font.loadFromFile("./assets/Arial.ttf")){
    //         sf::Text text("GAME OVER", font, 40);
    //         text.setFillColor(sf::Color::Red);
    //         text.setPosition(WINDOW_SIZE / 2 - 100, WINDOW_SIZE / 2 - 20);
    //         window.draw(text);
    //         window.display();
    //     }
    // }
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
    float timeRemaining = fastModeUntil - fastEffectClock.getElapsedTime().asSeconds();
    bool fastEndingSoon = timeRemaining > 0.0f && timeRemaining < 1.0f;
    bool blinkVisible = true;

    if(fastEndingSoon){
        float blinkCycle = std::fmod(fastEffectClock.getElapsedTime().asSeconds(), 0.2f);
        blinkVisible = blinkCycle < 0.1f;
    }

    sf::Color snakeColor = (fastEffectClock.getElapsedTime().asSeconds() < fastModeUntil) ? sf::Color(100, 255, 255) : sf::Color::Green;
    snakeBody.setFillColor(snakeColor);

    if(blinkVisible){
        for(const auto& segment : snake){
            snakeBody.setPosition(segment.x * CELL_SIZE, segment.y * CELL_SIZE);
            window.draw(snakeBody);
        }
    }
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed){
    if(!isPressed) return;

    if(key == sf::Keyboard::Escape){
        window.close();
    } else if(!gameStarted and !gameOver){
        if (key == sf::Keyboard::Up && direction.y != 1){
            gameStarted = true;
            direction = {0, -1};
        } else if (key == sf::Keyboard::Down && direction.y != -1){
            gameStarted = true;
            direction = {0, 1};
        } else if (key == sf::Keyboard::Left && direction.x != 1){
            gameStarted = true;
            direction = {-1, 0};
        } else if (key == sf::Keyboard::Right && direction.x != -1){
            gameStarted = true;
            direction = {1, 0};
        }
    } else if(gameStarted && !gameOver){
        if (key == sf::Keyboard::Up && direction.y != 1){
            direction = {0, -1};
        } else if (key == sf::Keyboard::Down && direction.y != -1){
            direction = {0, 1};
        } else if (key == sf::Keyboard::Left && direction.x != 1){
            direction = {-1, 0};
        } else if (key == sf::Keyboard::Right && direction.x != -1){
            direction = {1, 0};
        }
    }
}

void Game::spawnApple(){
    while(true){
        int x = rand() % GRID_SIZE;
        int y = rand() % GRID_SIZE;
        sf::Vector2i newPos(x, y);
        bool onSnake = false;

        for(const auto& segment : snake){
            if(segment == newPos){
                onSnake = true;
                break;
            }
        }

        if(!onSnake){
            applePosition = newPos;
            break;
        }
    }

    int roll = rand() % 100;
    if(roll < 80){
        currentAppleType = NORMAL;
    } else if(roll < 95){
        currentAppleType = GOLD;
    } else{
        currentAppleType = FAST;
    }

    appleTimer.restart();
}

void Game::drawApple(){
    float timeAlive = appleTimer.getElapsedTime().asSeconds();
    float timeLeft = APPLE_LIFESPAN - timeAlive;

    bool shouldDraw = true;

    if((currentAppleType == GOLD || currentAppleType == FAST) && timeLeft < 2.0f){
        float blinkCycle = std::fmod(timeAlive, 0.4f);
        shouldDraw = blinkCycle < 0.2f;
    }

    if(!shouldDraw) return;

    sf::RectangleShape apple(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    apple.setPosition(applePosition.x * CELL_SIZE, applePosition.y * CELL_SIZE);

    switch(currentAppleType){
        case NORMAL:
            apple.setFillColor(sf::Color::Red);
            break;
        case GOLD:
            apple.setFillColor(sf::Color(255, 215, 0));
            break;
        case FAST:
            apple.setFillColor(sf::Color::Cyan);
            break;
    }
    window.draw(apple);
}

void Game::resetGame(){
    snake.clear();
    sf::Vector2i startPos = {GRID_SIZE / 2, GRID_SIZE / 2};
    snakeLenght = 1;
    snake.push_back(startPos);
    direction = {1, 0};
    gameOver = false;
    gameStarted = false;
    score = 0;
    spawnApple();
}