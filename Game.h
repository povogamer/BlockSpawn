#pragma once

#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
/*
Class that acts as the game engine.
Wrapper class
*/

class Game
{
    private:
        // Variables
        // Window
        sf::RenderWindow* window;
        sf::VideoMode videoMode;
        sf::Event ev;

        // Game objects
        std::vector<sf::RectangleShape> enemies;
        sf::RectangleShape enemy;

        //Resources
        sf::Font font;

        //Text
        sf::Text uiText;

        // Game Logic
        bool endGame;
        int points;
        int health;
        float spawnEnemyTimer;
        float spawnEnemyTimerMax;
        int maxEnemies;
        bool mouseHeld;

        // Mouse Position
        sf::Vector2i mousePosWindow;
        sf::Vector2f mousePosView;

        ///Private functions
        void initVariables();
        void initWindow();
        void initEnemies();
        void initFonts();
        void initTexts();

    public:
        //Constructor / Destructors
        Game();
        virtual ~Game();

        // Accessors
        const bool running() const;
        const bool getEndGame() const;

        //Functions
        void spawnEnemies();
        void pollEvents();

        void updateMousePosition();
        void update();
        void updateEnemies();
        void updateText();

        void renderText(sf::RenderTarget& target);
        void renderEnemies(sf::RenderTarget& target);
        void render();
};
