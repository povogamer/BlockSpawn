#include "Game.h"

/**** Private Functions ****/
#pragma region Private_Functions
void Game::initVariables()
{
    this->window = nullptr;

    // Game Variables
    this->endGame = false;
    this->points = 0;
    this->spawnEnemyTimerMax = 10.f;
    //this->spawnEnemyTimer = 0.f;
    this->spawnEnemyTimer = this->spawnEnemyTimerMax;
    this->maxEnemies = 10;
    this->mouseHeld = false;
    this->health = 10;
}

void Game::initWindow()
{
    this->videoMode.height = 600;
    this->videoMode.width = 800;

    this->window = new sf::RenderWindow(sf::VideoMode(640, 480), "My first game", sf::Style::Titlebar | sf::Style::Close);
    this->window->setFramerateLimit(60);
}

void Game::initEnemies()
{
    this->enemy.setPosition(10.f, 10.f);
    this->enemy.setSize(sf::Vector2f(100.f, 100.f));
    //this->enemy.setScale(sf::Vector2f(0.5f, 0.5f));
    this->enemy.setFillColor(sf::Color::Cyan);
    //this->enemy.setOutlineColor(sf::Color::Green);
    //this->enemy.setOutlineThickness(1.f);
}

void Game::initFonts()
{
    if(!this->font.loadFromFile("/home/povo/workspace/sfml/Fonts/Roboto/Roboto-Black.ttf"))
    {
        std::cout << "ERROR:GAME::INITFONTS::Failed to load font!" << "\n";
        std::cout << "looking for: Fonts/Roboto/Roboto-Black.ttf" << "\n";
    }

}

void Game::initTexts()
{
    this->uiText.setFont(this->font);
    this->uiText.setCharacterSize(12);
    this->uiText.setFillColor(sf::Color::White);
    this->uiText.setString("NONE");
}
#pragma endregion Private_Functions

/**** Constructors / Destructors ****/
#pragma region Constructors_Desructors
Game::Game()
{
    this->initVariables();
    this->initWindow();
    this->initEnemies();
    this->initFonts();
    this->initTexts();
}

Game::~Game()
{
    delete this->window;
}
#pragma endregion Constructors_Desructors

/**** Accesssors ****/
#pragma region Accessors
const bool Game::running() const
{
    return this->window->isOpen();
}
const bool Game::getEndGame() const
{
    return this->endGame;
}
#pragma endregion Accessors

/**** Functions ****/
#pragma region Functions
void Game::spawnEnemies()
{
    /*
    @return void
    spawn enemies
    at location
    with color
    add enemy to vector*/

    this->enemy.setPosition(
        static_cast<float> (rand() % static_cast<int>(this->window->getSize().x)),
        0
    );

    //Randomize enemy type
    int type = rand() % 5;
    switch(type)
    {
        case 0:
            this->enemy.setSize(sf::Vector2f(15.f, 15.f));
            this->enemy.setFillColor(sf::Color::Magenta);
            break;
        case 1:
            this->enemy.setSize(sf::Vector2f(30.f, 30.f));
            this->enemy.setFillColor(sf::Color::Blue);
            break;
        case 2:
            this->enemy.setSize(sf::Vector2f(50.f, 50.f));
            this->enemy.setFillColor(sf::Color::Cyan);
            break;
        case 3:
            this->enemy.setSize(sf::Vector2f(70.f, 70.f));
            this->enemy.setFillColor(sf::Color::Red);
            break;
        case 4:
            this->enemy.setSize(sf::Vector2f(100.f, 100.f));
            this->enemy.setFillColor(sf::Color::Green);
            break;
        default:
            this->enemy.setSize(sf::Vector2f(100.f, 100.f));
            this->enemy.setFillColor(sf::Color::Yellow);
            break;
    }
    //Spawn the enemy
    this->enemies.push_back(this->enemy);
}

void Game::pollEvents()
{
    while(this->window->pollEvent(this->ev))
    {
        switch (this->ev.type)
        {
            case sf::Event::Closed:
                this->window->close();
                break;
            case sf::Event::KeyReleased:
                if (ev.key.code == sf::Keyboard::Escape)
                    this->window->close();
                break;

        }
    }
}

void Game::updateMousePosition()
{
    /*
    @return void
    
    updates mouse positions
        mouse position relative to window
    */
   this->mousePosWindow = sf::Mouse::getPosition(*this->window);
   this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Game::updateText()
{
    std::stringstream ss;

    ss << "Points: " << this->points << "\n"
        << "Health: " << this->health << "\n";

    this->uiText.setString(ss.str());
}

void Game::update()
{
    this->pollEvents();

    if (!this->endGame)
    {
        this->updateMousePosition();
        this->updateText();
        this->updateEnemies();
    }

    if(this->health <= 0)
    {
        this->endGame = true;
    }
}

void Game::updateEnemies()
{
    /*
    @return void
    
    updates enemy timer to spawn enemies
    */
    if (this->enemies.size() < this->maxEnemies)
    {
        if (this->spawnEnemyTimer >= this->spawnEnemyTimerMax)
        {   
            this->spawnEnemies();
            this->spawnEnemyTimer = 0.f;
        }
        else
            this->spawnEnemyTimer += 1.f;
    }

    // Move and updating the enemies
    for (int i = 0; i < this->enemies.size(); i++)
    {
        bool deleted = false;
        this->enemies[i].move(0.f, 2.f);

        // if the enemy is past the bottom of the screen
        if(this->enemies[i].getPosition().y > this->window->getSize().y)
        {
            this->enemies.erase(this->enemies.begin() + i);
            this->health -= 1;
            std::cout << "Health: " << this->health << "\n";
        }
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        if (this->mouseHeld == false)
        {
            this->mouseHeld = true;
            bool deleted = false;
            for (size_t i = 0; (i < this->enemies.size()) && (deleted == false); i++)
            {
                if (this->enemies[i].getGlobalBounds().contains(this->mousePosView))
                {
                    // Gain points
                    if(this->enemies[i].getFillColor() == sf::Color::Magenta)
                        this->points +=10;
                    else if (this->enemies[i].getFillColor() == sf::Color::Blue)
                        this->points += 7;
                    else if (this->enemies[i].getFillColor() == sf::Color::Cyan)
                        this->points += 5;
                    else if (this->enemies[i].getFillColor() == sf::Color::Red)
                        this->points += 3;
                    else if (this->enemies[i].getFillColor() == sf::Color::Green)
                        this->points += 1;
                    
                    std::cout << "Points: " << this->points << "\n";

                    deleted = true;
                    this->enemies.erase(this->enemies.begin() + i);
                }
            }
        }
    }
    else
    {
        this->mouseHeld = false;
    }
}

void Game::renderEnemies(sf::RenderTarget& target)
{
    // rendering all the enemies
    for (auto &e : this->enemies)
    {
        target.draw(e);
    }
}

void Game::renderText(sf::RenderTarget& target)
{
    target.draw(this->uiText);
}

void Game::render()
{
    /*
    @return void

    - clear old frame
    - render objects
    - display frame in window

    Renders the game objects.*/
    this->window->clear();
    
    //Draw game objects
    this->renderEnemies(*this->window);
    this->renderText(*this->window);

    this->window->display();
}
#pragma endregion Functions