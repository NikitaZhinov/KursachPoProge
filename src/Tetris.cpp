#include "Tetris.h"

namespace tetris {

    Tetris::Tetris() {
        screen_size.width = 400;
        screen_size.height = 600;

        initBrickGame();
    }

    Tetris::~Tetris() {
        delete[] brickgame_buttons;
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

            window.display();
        }
    }

    void Tetris::initBrickGame() {
        initBrickGameBackground();

        brickgame_buttons = new sf::CircleShape[brickgame_buttons_count + 1];
        for (int i = 0; i < brickgame_buttons_count; i++) {
            brickgame_buttons[i].setRadius(brickgame_buttons_radius);
            brickgame_buttons[i].setPosition(sf::Vector2f(brickgame_buttons_pos.x + brickgame_buttons_distance * std::cos(3.1415926535 / 2. * i),
                                                          brickgame_buttons_pos.y + brickgame_buttons_distance * std::sin(3.1415926535 / 2. * i)));
            brickgame_buttons[i].setFillColor(sf::Color(brickgame_buttons_color.r,
                                                        brickgame_buttons_color.g,
                                                        brickgame_buttons_color.b,
                                                        brickgame_buttons_color.a));
        }
        brickgame_buttons[brickgame_buttons_count].setRadius(brickgame_buttons_radius * 1.5);
        brickgame_buttons[brickgame_buttons_count].setPosition(sf::Vector2f(brickgame_buttons_pos.x * 2.2,
                                                                            brickgame_buttons_pos.y - brickgame_buttons_distance));
        brickgame_buttons[brickgame_buttons_count].setFillColor(sf::Color(brickgame_buttons_color.r,
                                                                          brickgame_buttons_color.g,
                                                                          brickgame_buttons_color.b,
                                                                          brickgame_buttons_color.a));

        brickgame_screen.setSize(sf::Vector2f(brickgame_screen_size.width, brickgame_screen_size.height));
        brickgame_screen.setPosition(sf::Vector2f(brickgame_screen_pos.x, brickgame_screen_pos.y));
        brickgame_screen.setFillColor(sf::Color(brickgame_screen_color.r,
                                                brickgame_screen_color.g,
                                                brickgame_screen_color.b,
                                                brickgame_screen_color.a));
    }

    void Tetris::initBrickGameBackground() {
        brickgame_background.setSize(sf::Vector2f(screen_size.width, screen_size.height));
        brickgame_background.setFillColor(sf::Color(255, 255, 0, 255));
    }

    void Tetris::draw(sf::RenderWindow &window) {
        window.draw(brickgame_background);
        for (int i = 0; i < brickgame_buttons_count + 1; i++)
            window.draw(brickgame_buttons[i]);
        window.draw(brickgame_screen);
    }

    void Tetris::logic() {
    }
} // namespace tetris