#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

namespace tetris {
    struct Size_t {
        int width, height;
    };

    struct Point_t {
        int x, y;
    };

    struct Color_t {
        int r, g, b, a;
    };

    class Tetris {
    private:
        Size_t screen_size;

        sf::RectangleShape brickgame_background;

        sf::CircleShape *brickgame_buttons;
        int brickgame_buttons_count = 4;
        float brickgame_buttons_radius = 30;
        Point_t brickgame_buttons_pos = { 120, 425 };
        float brickgame_buttons_distance = 70;
        Color_t brickgame_buttons_color = { 0, 0, 255, 255 };

        sf::RectangleShape brickgame_screen;
        Color_t brickgame_screen_color = { 165, 199, 179, 255 };
        Size_t brickgame_screen_size = { 260, 260 };
        Point_t brickgame_screen_pos = { 70, 50 };

        void draw(sf::RenderWindow &window);
        void logic();
        void initBrickGame();
        void initBrickGameBackground();

    public:
        Tetris();
        ~Tetris();

        void run();
    };

} // namespace tetris