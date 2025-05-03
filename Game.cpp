#include "Game.h"
#include <cstdlib>
#include <ctime>

Game::Game() : window(sf::VideoMode(600, 600), L"Snake Game") {
    sf::Vector2i startPos = {GRID_SIZE / 2, GRID_SIZE / 2};
    for(int i=0; i<snakeLenght; ++i){
        snake.push_back({startPos.x - i, startPos.y});
    }
    snakeBody.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    snakeBody.setFillColor(sf::Color::Green);
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
        spawnApple();
    }

    snake.push_front(newHead);
    if(snake.size() > snakeLenght){
        snake.pop_back();
    }
}

void Game::render() {
    window.clear(sf::Color::Black);
    
    drawGrid();
    drawSnake();
    drawApple();
    
    window.display();

    // if(gameOver){
    //     sf::Font font;
    //     if(font.loadFromFile("arial.ttf")){
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
    for(const auto& segment : snake){
        snakeBody.setPosition(segment.x * CELL_SIZE, segment.y * CELL_SIZE);
        window.draw(snakeBody);
    }
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed){
    if(!isPressed) return;

    if(key == sf::Keyboard::Escape){
        window.close();
    } else if(!gameStarted){
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
    } else{
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
    std::srand(std::time(nullptr));
    while(true){
        int x = std::rand() % GRID_SIZE;
        int y = std::rand() % GRID_SIZE;
        sf::Vector2i potentialPos = {x, y};

        bool onSnake = false;
        for(const auto& segment : snake){
            if(segment == potentialPos){
                onSnake = true;
                break;
            }
        }

        if(!onSnake){
            applePosition = potentialPos;
            break;
        }
    }
}

void Game::drawApple(){
    appleShape.setPosition(applePosition.x * CELL_SIZE, applePosition.y * CELL_SIZE);
    window.draw(appleShape);
}

void Game::resetGame(){
    snake.clear();
    sf::Vector2i startPos = {GRID_SIZE / 2, GRID_SIZE / 2};
    snakeLenght = 1;
    snake.push_back(startPos);
    direction = {1, 0};
    gameOver = false;
    gameStarted = false;
    spawnApple();
}