#include "Tetris.h"

Tetris::Tetris() {
    initBrickGame();
    initGame();
}

Tetris::~Tetris() {
    delete[] brickgame_buttons;
    removeBrick(current_brick);
    removeBrick(next_brick);
    for (int i = 0; i < size_field.height; i++)
        delete[] field[i];
    delete[] field;
}

void Tetris::run() {
    sf::RenderWindow window(sf::VideoMode(screen_size.width, screen_size.height), "Tetris");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        window.clear();

        draw(window);
        logic();
        getAction();

        window.display();
    }
}

void Tetris::initBrickGame() {
    initBrickGameBackground();
    initBrickGameButtons();
    initBrickGameScreen();
}

void Tetris::initBrickGameBackground() {
    brickgame_background.setSize(sf::Vector2f(screen_size.width, screen_size.height));
    brickgame_background.setFillColor(sf::Color(255, 255, 0, 255));
}

void Tetris::initBrickGameButtons() {
    brickgame_buttons = new sf::CircleShape[brickgame_buttons_count + 1];
    for (int i = 0; i < brickgame_buttons_count; i++) {
        brickgame_buttons[i].setRadius(brickgame_buttons_radius);
        brickgame_buttons[i].setPosition(sf::Vector2f(brickgame_buttons_pos.x + brickgame_buttons_distance * std::cos(3.1415926535 / 2. * i),
                                                      brickgame_buttons_pos.y + brickgame_buttons_distance * std::sin(3.1415926535 / 2. * i)));
        brickgame_buttons[i].setFillColor(sf::Color(brickgame_buttons_color.r,
                                                    brickgame_buttons_color.g,
                                                    brickgame_buttons_color.b));
    }
    brickgame_buttons[brickgame_buttons_count].setRadius(brickgame_buttons_radius * 1.5);
    brickgame_buttons[brickgame_buttons_count].setPosition(sf::Vector2f(brickgame_buttons_pos.x * 2.2,
                                                                        brickgame_buttons_pos.y - brickgame_buttons_distance));
    brickgame_buttons[brickgame_buttons_count].setFillColor(sf::Color(brickgame_buttons_color.r,
                                                                      brickgame_buttons_color.g,
                                                                      brickgame_buttons_color.b));
}

void Tetris::initBrickGameScreen() {
    brickgame_screen.setSize(sf::Vector2f(brickgame_screen_size.width, brickgame_screen_size.height));
    brickgame_screen.setPosition(sf::Vector2f(brickgame_screen_pos.x, brickgame_screen_pos.y));
    brickgame_screen.setFillColor(sf::Color(brickgame_screen_color.r,
                                            brickgame_screen_color.g,
                                            brickgame_screen_color.b));
}

void Tetris::draw(sf::RenderWindow &window) {
    window.draw(brickgame_background);
    for (int i = 0; i < brickgame_buttons_count + 1; i++)
        window.draw(brickgame_buttons[i]);
    window.draw(brickgame_screen);
    drawField(window);
    drawBrick(window);
}

void Tetris::drawField(sf::RenderWindow &window) {
    for (int i = 0; i < size_field.height; i++)
        for (int j = 0; j < size_field.width; j++)
            if (field[i][j].i)
                window.draw(field[i][j].pixel);
}

void Tetris::drawBrick(sf::RenderWindow &window) {
    for (int i = 0; i < current_brick.size.height; i++)
        for (int j = 0; j < current_brick.size.width; j++)
            if (current_brick.matrix[i][j].i)
                window.draw(current_brick.matrix[i][j].pixel);
}

void Tetris::logic() {
    updateField();
    updateBrick();
}

void Tetris::getAction() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) &&
        timer_move.getElapsedTime().asMilliseconds() / delay_move > speed / level) {
        rotateBrick();
        timer_move.restart();
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        moveLeftBrick();
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        moveRightBrick();
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        toDownBrick();
}

void Tetris::initGame() {
    initField();
    initBrick(current_brick);
    initBrick(next_brick);
}

void Tetris::initField() {
    field = new PointField *[size_field.height];
    pixel.setSize(sf::Vector2f(size_pixel.width, size_pixel.height));
    for (int i = 0; i < size_field.height; i++) {
        field[i] = new PointField[size_field.width];
        for (int j = 0; j < size_field.width; j++) {
            setPixel({ j * size_pixel.width,
                       i * size_pixel.height },
                     pixel_color);
            field[i][j] = { j, i, 0, pixel };
        }
    }
}

void Tetris::initBrick(Brick_t &brick) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, count_bricks - 1);

    brick.pos = { 4, 0 };
    brick.rotate_mode = 0;
    setPixelColor(pixel_color);

    switch (dist(gen)) {
        case 0:
            initRightStick(brick);
            break;

        case 1:
            initLeftStick(brick);
            break;

        case 2:
            initRightStep(brick);
            break;

        case 3:
            initLeftStep(brick);
            break;

        case 4:
            initCube(brick);
            break;

        case 5:
            initStick(brick);
            break;

        case 6:
            initTriangle(brick);
            break;
    }
}

void Tetris::initRightStick(Brick_t &brick) {
    brick.number = RightStick;
    brick.size = { 2, 3 };
    brick.matrix = new PointField *[brick.size.height];
    for (int i = 0; i < brick.size.height; i++) {
        brick.matrix[i] = new PointField[brick.size.width];
        brick.matrix[i][0] = { 0, i, 1, pixel };
        brick.matrix[i][1] = { 1, i, 0, pixel };
    }
    brick.matrix[0][1] = { 1, 0, 1, pixel };
}

void Tetris::initLeftStick(Brick_t &brick) {
    brick.number = LeftStick;
    brick.size = { 2, 3 };
    brick.matrix = new PointField *[brick.size.height];
    for (int i = 0; i < brick.size.height; i++) {
        brick.matrix[i] = new PointField[brick.size.width];
        brick.matrix[i][1] = { 1, i, 1, pixel };
        brick.matrix[i][0] = { 0, i, 0, pixel };
    }
    brick.matrix[0][0] = { 0, 0, 1, pixel };
}

void Tetris::initRightStep(Brick_t &brick) {
    brick.number = RightStep;
    brick.size = { 3, 2 };
    brick.matrix = new PointField *[brick.size.height];
    for (int i = 0; i < brick.size.height; i++)
        brick.matrix[i] = new PointField[brick.size.width];

    brick.matrix[0][0] = { 0, 0, 0, pixel };
    brick.matrix[1][2] = { 2, 1, 0, pixel };

    brick.matrix[0][1] = { 1, 0, 1, pixel };
    brick.matrix[0][2] = { 2, 0, 1, pixel };
    brick.matrix[1][0] = { 0, 1, 1, pixel };
    brick.matrix[1][1] = { 1, 1, 1, pixel };
}

void Tetris::initLeftStep(Brick_t &brick) {
    brick.number = LeftStep;
    brick.size = { 3, 2 };
    brick.matrix = new PointField *[brick.size.height];
    for (int i = 0; i < brick.size.height; i++)
        brick.matrix[i] = new PointField[brick.size.width];

    brick.matrix[0][2] = { 2, 0, 0, pixel };
    brick.matrix[1][0] = { 0, 1, 0, pixel };

    brick.matrix[0][0] = { 0, 0, 1, pixel };
    brick.matrix[0][1] = { 1, 0, 1, pixel };
    brick.matrix[1][1] = { 1, 1, 1, pixel };
    brick.matrix[1][2] = { 2, 1, 1, pixel };
}

void Tetris::initCube(Brick_t &brick) {
    brick.number = Cube;
    brick.size = { 2, 2 };
    brick.matrix = new PointField *[brick.size.height];
    setPixelColor(pixel_color);
    for (int i = 0; i < brick.size.height; i++) {
        brick.matrix[i] = new PointField[brick.size.width];
        for (int j = 0; j < brick.size.width; j++)
            brick.matrix[i][j] = { j, i, 1, pixel };
    }
}

void Tetris::initStick(Brick_t &brick) {
    brick.number = Stick;
    brick.size = { 1, 4 };
    brick.matrix = new PointField *[brick.size.height];
    setPixelColor(pixel_color);
    for (int i = 0; i < brick.size.height; i++) {
        brick.matrix[i] = new PointField[brick.size.width];
        brick.matrix[i][0] = { 0, i, 1, pixel };
    }
}

void Tetris::initTriangle(Brick_t &brick) {
    brick.number = Triangle;
    brick.size = { 3, 2 };
    brick.matrix = new PointField *[brick.size.height];
    for (int i = 0; i < brick.size.height; i++)
        brick.matrix[i] = new PointField[brick.size.width];

    brick.matrix[0][0] = { 0, 0, 0, pixel };
    brick.matrix[0][2] = { 2, 0, 0, pixel };

    brick.matrix[0][1] = { 1, 0, 1, pixel };
    for (int i = 0; i < brick.size.width; i++)
        brick.matrix[1][i] = { i, 1, 1, pixel };
}

void Tetris::setPixelPos(Point_t pos) {
    pixel.setPosition(sf::Vector2f(brickgame_screen_pos.x + pos.x, brickgame_screen_pos.y + pos.y));
}

void Tetris::setPixelColor(Color_t color) {
    pixel.setFillColor(sf::Color(color.r, color.g, color.b));
}

void Tetris::setPixel(Point_t pos, Color_t color) {
    pixel.setPosition(sf::Vector2f(brickgame_screen_pos.x + pos.x, brickgame_screen_pos.y + pos.y));
    pixel.setFillColor(sf::Color(color.r, color.g, color.b));
}

void Tetris::removeBrick(Brick_t &brick) {
    for (int i = 0; i < brick.size.height; i++)
        delete[] brick.matrix[i];
    delete[] brick.matrix;
}

void Tetris::updateField() {
    for (int i = 0; i < size_field.height; i++) {
        bool is_full = 1;
        for (int j = 0; j < size_field.width; j++)
            if (!field[i][j].i)
                is_full = 0;
        if (is_full) {
            for (int j = 0; j < size_field.width; j++)
                field[i][j].i = 0;
            score += 100;
            for (int y = i - 1; y >= 0; y--) {
                for (int x = 0; x < size_field.width; x++) {
                    field[y + 1][x].i = field[y][x].i;
                    field[y][x].i = 0;
                }
            }
        }
    }
}

int Tetris::updateBrick() {
    bool is_update = true;
    for (int i = 0; i < current_brick.size.height; i++)
        for (int j = 0; j < current_brick.size.width; j++)
            if (current_brick.pos.y + current_brick.size.height >= size_field.height ||
                (current_brick.matrix[i][j].i && field[current_brick.pos.y + i + 1][current_brick.pos.x + j].i))
                is_update = false;
    if (timer.getElapsedTime().asMilliseconds() / delay > speed / level) {
        if (is_update) {
            current_brick.pos.y++;
            timer.restart();
        } else {
            for (int i = 0; i < current_brick.size.height; i++) {
                int y = i + current_brick.pos.y;
                for (int j = 0; j < current_brick.size.width; j++)
                    if (current_brick.matrix[i][j].i)
                        field[y][j + current_brick.pos.x].i = true;
            }
            removeBrick(current_brick);
            current_brick = std::move(next_brick);
            initBrick(next_brick);
        }
    }

    updatePixelBrick();
    return is_update;
}

void Tetris::rotateBrick() {
    switch (current_brick.number) {
        case RightStick: {
            current_brick.rotate_mode = (current_brick.rotate_mode + 1) % 4;
            switch (current_brick.rotate_mode) {
                case 0: {
                    Point_t pos = current_brick.pos;
                    removeBrick(current_brick);
                    initRightStick(current_brick);
                    current_brick.pos = pos;
                } break;

                case 1: {
                    removeBrick(current_brick);
                    current_brick.size = { 3, 2 };
                    current_brick.matrix = new PointField *[current_brick.size.height];
                    for (int i = 0; i < current_brick.size.height; i++)
                        current_brick.matrix[i] = new PointField[current_brick.size.width];
                    for (int i = 0; i < current_brick.size.width; i++) {
                        current_brick.matrix[0][i] = { i, 0, 1, pixel };
                        current_brick.matrix[1][i] = { i, 1, 0, pixel };
                    }
                    current_brick.matrix[1][2] = { 2, 1, 1, pixel };
                } break;

                case 2: {
                    removeBrick(current_brick);
                    current_brick.size = { 2, 3 };
                    current_brick.matrix = new PointField *[current_brick.size.height];
                    for (int i = 0; i < current_brick.size.height; i++) {
                        current_brick.matrix[i] = new PointField[current_brick.size.width];
                        current_brick.matrix[i][1] = { 1, i, 1, pixel };
                        current_brick.matrix[i][0] = { 0, i, 0, pixel };
                    }
                    current_brick.matrix[2][0] = { 0, 2, 1, pixel };
                } break;

                case 3: {
                    removeBrick(current_brick);
                    current_brick.size = { 3, 2 };
                    current_brick.matrix = new PointField *[current_brick.size.height];
                    for (int i = 0; i < current_brick.size.height; i++)
                        current_brick.matrix[i] = new PointField[current_brick.size.width];
                    for (int i = 0; i < current_brick.size.width; i++) {
                        current_brick.matrix[1][i] = { i, 1, 1, pixel };
                        current_brick.matrix[0][i] = { i, 0, 0, pixel };
                    }
                    current_brick.matrix[0][0] = { 0, 0, 1, pixel };
                } break;
            }
        } break;

        case LeftStick: {
            current_brick.rotate_mode = (current_brick.rotate_mode + 1) % 4;
            switch (current_brick.rotate_mode) {
                case 0: {
                    Point_t pos = current_brick.pos;
                    removeBrick(current_brick);
                    initLeftStick(current_brick);
                    current_brick.pos = pos;
                } break;

                case 1: {
                    removeBrick(current_brick);
                    current_brick.size = { 3, 2 };
                    current_brick.matrix = new PointField *[current_brick.size.height];
                    for (int i = 0; i < current_brick.size.height; i++)
                        current_brick.matrix[i] = new PointField[current_brick.size.width];
                    for (int i = 0; i < current_brick.size.width; i++) {
                        current_brick.matrix[0][i] = { i, 0, 0, pixel };
                        current_brick.matrix[1][i] = { i, 1, 1, pixel };
                    }
                    current_brick.matrix[0][2] = { 2, 0, 1, pixel };
                } break;

                case 2: {
                    removeBrick(current_brick);
                    current_brick.size = { 2, 3 };
                    current_brick.matrix = new PointField *[current_brick.size.height];
                    for (int i = 0; i < current_brick.size.height; i++) {
                        current_brick.matrix[i] = new PointField[current_brick.size.width];
                        current_brick.matrix[i][1] = { 1, i, 0, pixel };
                        current_brick.matrix[i][0] = { 0, i, 1, pixel };
                    }
                    current_brick.matrix[2][1] = { 1, 2, 1, pixel };
                } break;

                case 3: {
                    removeBrick(current_brick);
                    current_brick.size = { 3, 2 };
                    current_brick.matrix = new PointField *[current_brick.size.height];
                    for (int i = 0; i < current_brick.size.height; i++)
                        current_brick.matrix[i] = new PointField[current_brick.size.width];
                    for (int i = 0; i < current_brick.size.width; i++) {
                        current_brick.matrix[1][i] = { i, 1, 0, pixel };
                        current_brick.matrix[0][i] = { i, 0, 1, pixel };
                    }
                    current_brick.matrix[1][0] = { 0, 1, 1, pixel };
                } break;
            }
        } break;

        case RightStep: {
            current_brick.rotate_mode = (current_brick.rotate_mode + 1) % 2;
            switch (current_brick.rotate_mode) {
                case 0: {
                    Point_t pos = current_brick.pos;
                    removeBrick(current_brick);
                    initRightStep(current_brick);
                    current_brick.pos = pos;
                } break;

                case 1: {
                    removeBrick(current_brick);
                    current_brick.size = { 2, 3 };
                    current_brick.matrix = new PointField *[current_brick.size.height];
                    for (int i = 0; i < current_brick.size.height; i++)
                        current_brick.matrix[i] = new PointField[current_brick.size.width];

                    current_brick.matrix[0][1] = { 1, 0, 0, pixel };
                    current_brick.matrix[2][0] = { 0, 2, 0, pixel };

                    current_brick.matrix[0][0] = { 0, 0, 1, pixel };
                    current_brick.matrix[1][0] = { 0, 1, 1, pixel };
                    current_brick.matrix[1][1] = { 1, 1, 1, pixel };
                    current_brick.matrix[2][1] = { 1, 2, 1, pixel };
                } break;
            }
        } break;

        case LeftStep: {
            current_brick.rotate_mode = (current_brick.rotate_mode + 1) % 2;
            switch (current_brick.rotate_mode) {
                case 0: {
                    Point_t pos = current_brick.pos;
                    removeBrick(current_brick);
                    initLeftStep(current_brick);
                    current_brick.pos = pos;
                } break;

                case 1: {
                    removeBrick(current_brick);
                    current_brick.size = { 2, 3 };
                    current_brick.matrix = new PointField *[current_brick.size.height];
                    for (int i = 0; i < current_brick.size.height; i++)
                        current_brick.matrix[i] = new PointField[current_brick.size.width];

                    current_brick.matrix[0][0] = { 0, 0, 0, pixel };
                    current_brick.matrix[2][1] = { 1, 2, 0, pixel };

                    current_brick.matrix[0][1] = { 1, 0, 1, pixel };
                    current_brick.matrix[1][1] = { 1, 1, 1, pixel };
                    current_brick.matrix[1][0] = { 0, 1, 1, pixel };
                    current_brick.matrix[2][0] = { 0, 2, 1, pixel };
                } break;
            }
        } break;

        case Cube:
            break;

        case Stick: {
            current_brick.rotate_mode = (current_brick.rotate_mode + 1) % 2;
            switch (current_brick.rotate_mode) {
                case 0: {
                    Point_t pos = current_brick.pos;
                    removeBrick(current_brick);
                    initStick(current_brick);
                    current_brick.pos = pos;
                } break;

                case 1: {
                    removeBrick(current_brick);
                    current_brick.size = { 4, 1 };
                    current_brick.matrix = new PointField *[current_brick.size.height];
                    for (int i = 0; i < current_brick.size.height; i++) {
                        current_brick.matrix[i] = new PointField[current_brick.size.width];
                        for (int j = 0; j < current_brick.size.width; j++)
                            current_brick.matrix[0][j] = { j, 0, 1, pixel };
                    }
                } break;
            }
        } break;

        case Triangle: {
            current_brick.rotate_mode = (current_brick.rotate_mode + 1) % 4;
            switch (current_brick.rotate_mode) {
                case 0: {
                    Point_t pos = current_brick.pos;
                    removeBrick(current_brick);
                    initTriangle(current_brick);
                    current_brick.pos = pos;
                } break;

                case 1: {
                    removeBrick(current_brick);
                    current_brick.size = { 2, 3 };
                    current_brick.matrix = new PointField *[current_brick.size.height];
                    for (int i = 0; i < current_brick.size.height; i++)
                        current_brick.matrix[i] = new PointField[current_brick.size.width];

                    current_brick.matrix[0][1] = { 1, 0, 0, pixel };
                    current_brick.matrix[2][1] = { 1, 2, 0, pixel };

                    current_brick.matrix[1][1] = { 1, 1, 1, pixel };
                    for (int i = 0; i < current_brick.size.height; i++)
                        current_brick.matrix[i][0] = { 0, i, 1, pixel };
                } break;

                case 2: {
                    removeBrick(current_brick);
                    current_brick.size = { 3, 2 };
                    current_brick.matrix = new PointField *[current_brick.size.height];
                    for (int i = 0; i < current_brick.size.height; i++)
                        current_brick.matrix[i] = new PointField[current_brick.size.width];

                    current_brick.matrix[1][0] = { 0, 1, 0, pixel };
                    current_brick.matrix[1][2] = { 2, 1, 0, pixel };

                    current_brick.matrix[1][1] = { 1, 1, 1, pixel };
                    for (int i = 0; i < current_brick.size.width; i++)
                        current_brick.matrix[0][i] = { i, 0, 1, pixel };
                } break;

                case 3: {
                    removeBrick(current_brick);
                    current_brick.size = { 2, 3 };
                    current_brick.matrix = new PointField *[current_brick.size.height];
                    for (int i = 0; i < current_brick.size.height; i++)
                        current_brick.matrix[i] = new PointField[current_brick.size.width];

                    current_brick.matrix[0][0] = { 0, 0, 0, pixel };
                    current_brick.matrix[2][0] = { 0, 2, 0, pixel };

                    current_brick.matrix[1][0] = { 0, 1, 1, pixel };
                    for (int i = 0; i < current_brick.size.height; i++)
                        current_brick.matrix[i][1] = { 1, i, 1, pixel };
                } break;
            }
        } break;
    }

    if (current_brick.pos.x + current_brick.size.width > size_field.width)
        current_brick.pos.x -= current_brick.size.width - 1;
}

void Tetris::moveLeftBrick() {
    bool is_move = true;
    for (int i = 0; i < current_brick.size.height; i++) {
        for (int j = 0; j < current_brick.size.width; j++) {
            if (current_brick.pos.x <= 0 ||
                (current_brick.matrix[i][j].i && field[current_brick.pos.y + i][current_brick.pos.x + j - 1].i)) {
                is_move = false;
                break;
            }
        }
    }
    if (is_move && timer_move.getElapsedTime().asMilliseconds() / delay_move > speed / level) {
        current_brick.pos.x--;
        timer_move.restart();
    }
}

void Tetris::moveRightBrick() {
    bool is_move = true;
    for (int i = 0; i < current_brick.size.height; i++) {
        for (int j = 0; j < current_brick.size.width; j++) {
            if (current_brick.pos.x + current_brick.size.width >= size_field.width ||
                (current_brick.matrix[i][j].i && field[current_brick.pos.y + i][current_brick.pos.x + j + 1].i)) {
                is_move = false;
                break;
            }
        }
    }
    if (is_move && timer_move.getElapsedTime().asMilliseconds() / delay_move > speed / level) {
        current_brick.pos.x++;
        timer_move.restart();
    }
}

void Tetris::toDownBrick() {
    while (updateBrick()) {
        current_brick.pos.y++;
        timer.restart();
    }
}

void Tetris::updatePixelBrick() {
    for (int i = 0; i < current_brick.size.height; i++)
        for (int j = 0; j < current_brick.size.width; j++)
            current_brick.matrix[i][j].pixel.setPosition(sf::Vector2f(brickgame_screen_pos.x + (j + current_brick.pos.x) * size_pixel.width,
                                                                      brickgame_screen_pos.y + (i + current_brick.pos.y) * size_pixel.height));
}
