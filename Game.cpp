#include "Game.h"
#include <cstdlib>
#include <ctime>
#include <cmath>

Game::Game() : window(sf::VideoMode(600, 600), L"Snake Game"){
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

    obstacles.push_back({5, 5});
    obstacles.push_back({15, 15});

    spawnInitialApples();
}

void Game::run(){
    sf::Clock clock;

    while (window.isOpen()) {
        processEvents();

        float deltaTime = clock.restart().asSeconds();

        while(movementClock.getElapsedTime().asSeconds() >= moveDelay){
            update();
            movementClock.restart();
        }

        render();
    }
}

void Game::processEvents(){
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

void Game::update(){
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

    for(const auto& obs : obstacles){
        if(newHead == obs){
            gameOver = true;
            return;
        }
    }

    for(size_t i=0; i < apples.size(); ++i){
        if(newHead == apples[i].position){
            snakeLenght++;

            switch(apples[i].type){
                case NORMAL:
                    score += 1;
                    break;
                case GOLD:
                    score += 3;
                    break;
                case FAST:
                    score += 1;
                    fastModeUntil = fastEffectClock.getElapsedTime().asSeconds() + 5.0f;
                    moveDelay = 0.06f;
                    break;
            }

            apples.erase(apples.begin() + i);
            spawnApple();
            break;
        }
    }

    if(score >= nextObstacleScore){
        addRandomObstacle();
        nextObstacleScore += 5;
    }

    for(int i=apples.size() - 1; i >= 0; --i){
        if((apples[i].type == GOLD || apples[i].type == FAST) && apples[i].timer.getElapsedTime().asSeconds() > APPLE_LIFESPAN){
            apples.erase(apples.begin() + i);
            spawnApple();
        }
    }

    snake.push_front(newHead);
    if(snake.size() > snakeLenght){
        snake.pop_back();
    }
}

void Game::render(){
    bool fastActive = fastEffectClock.getElapsedTime().asSeconds() <= fastModeUntil;
    window.clear(fastActive ? sf::Color(20, 20, 40) : sf::Color::Black);
    
    drawGrid();
    drawObstacles();
    drawSnake();
    drawApples();

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
        if(key == sf::Keyboard::Up && direction.y != 1){
            gameStarted = true;
            direction = {0, -1};
        } else if(key == sf::Keyboard::Down && direction.y != -1){
            gameStarted = true;
            direction = {0, 1};
        } else if(key == sf::Keyboard::Left && direction.x != 1){
            gameStarted = true;         
            direction = {-1, 0};
        } else if(key == sf::Keyboard::Right && direction.x != -1){
            gameStarted = true;         
            direction = {1, 0};
        }
        for(auto& a : apples){
            a.timer.restart();
        }
    } else if(gameStarted && !gameOver){
        if(key == sf::Keyboard::Up && direction.y != 1){
            direction = {0, -1};
        } else if(key == sf::Keyboard::Down && direction.y != -1){
            direction = {0, 1};
        } else if(key == sf::Keyboard::Left && direction.x != 1){
            direction = {-1, 0};
        } else if(key == sf::Keyboard::Right && direction.x != -1){
            direction = {1, 0};
        }
    }
}

void Game::spawnApple(){
    for(int attempts=0; attempts < 100; ++attempts){
        int x = rand() % GRID_SIZE;
        int y = rand() % GRID_SIZE;
        sf::Vector2i pos(x, y);
        bool conflict = false;

        for(const auto& segment : snake){
            if(segment == pos) conflict = true;
        }
        for(const auto& obs : obstacles){
            if(obs == pos) conflict = true;
        }
        for(const auto& a : apples){
            if(a.position == pos) conflict = true;
        }

        if(!conflict){
            Apple newApple;
            newApple.position = pos;

            int roll = rand() % 100;
            if(roll < 50){
                newApple.type = NORMAL;
            } else if(roll < 95){
                newApple.type = GOLD;
            } else{
                newApple.type = FAST;
            }
            newApple.timer.restart();
            apples.push_back(newApple);
            return;
        }
    }
}

void Game::drawApples(){
    for(const auto& apple : apples){
        float timeAlive = apple.timer.getElapsedTime().asSeconds();
        float timeLeft = APPLE_LIFESPAN - timeAlive;
        bool shouldDraw = true;

        if((apple.type == GOLD || apple.type == FAST) && timeLeft < 2.0f){
            float blinkCycle = std::fmod(timeAlive, 0.4f);
            if(blinkCycle >= 0.2f) continue;
        }
        
        sf::RectangleShape rect(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        rect.setPosition(apple.position.x * CELL_SIZE, apple.position.y * CELL_SIZE);
        
        switch(apple.type){
            case NORMAL:
                rect.setFillColor(sf::Color::Red);
                break;
            case GOLD:
                rect.setFillColor(sf::Color(255, 215, 0));
                break;
            case FAST:
                rect.setFillColor(sf::Color::Cyan);
                break;
        }
        window.draw(rect);
    }
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
    apples.clear();
    spawnInitialApples();
}

void Game::drawObstacles(){
    sf::RectangleShape block(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    block.setFillColor(sf::Color(128, 128, 128));

    for(const auto& obs : obstacles){
        block.setPosition(obs.x * CELL_SIZE, obs.y * CELL_SIZE);
        window.draw(block);
    }
}

void Game::addRandomObstacle(){
    for(int attempts = 0; attempts < 100; ++attempts){
        int x = rand() % GRID_SIZE;
        int y = rand() % GRID_SIZE;
        sf::Vector2i pos(x, y);
        bool conflict = false;

        for(const auto& segment : snake){
            if(segment == pos) conflict = true;
        }
        for(const auto& obs : obstacles){
            if(obs == pos) conflict = true;
        }
        for(const auto& a : apples){
            if(a.position == pos) conflict = true;
        }

        if(!conflict){
            obstacles.push_back(pos);
            return;
        }
    }
}

void Game::spawnInitialApples(){
    apples.clear();
    for(int i=0; i < MAX_APPLES; ++i){
        spawnApple();
    }
}