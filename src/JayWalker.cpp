
#include <SFML/Graphics.hpp>
#include "JayWalker.h"

JayWalker::JayWalker(sf::Vector2f frogSize, sf::Texture& imageTexture, sf::Vector2u imageCount, float switchTime)
: Obstacle(imageTexture, frogSize), Animation(getTexture(), imageCount, switchTime)
{
    cout << "Jay walker created." << endl;
    changeLives = -1;
    timer = sf::Time::Zero;
    obstacleId = jaywalker;
}

JayWalker::~JayWalker(){}

bool JayWalker::move(float deltaTime)
{
    
    timer = ck.restart();
    
    sf::Vector2f pos = getPosition();
    
    // random movement
    totalTime += deltaTime/3;
    if (totalTime > switchTime) {
        totalTime -= switchTime;
        switch (rand()%2) {
            case 0:
                faceRight = true;
                if(pos.x <= 824.0f)
                    movement = sf::Vector2f(Animation::getHorizontalSpeed(), 0);
                else
                    movement = sf::Vector2f(0,0);
                break;
            case 1:
                faceRight = false;
                if(pos.x >= 220.0f)
                    movement = sf::Vector2f(-Animation::getHorizontalSpeed(), 0);
                else
                    movement = sf::Vector2f(0,0);
                break;
            default:;
        }
    }
    
    if(movement.x == 0.0f && movement.y > 0)
    {
        row = 0;        // face down
    }
    else if(movement.y < 0)
    {
        row = 3;        // face up
    }
    else if (movement.x != 0.0f && movement.y == 0)
    {
        row = 2;        // face right or left
    }
    setTextureRect(update_uvRect(deltaTime));
    
    if (pos.y < 768 + 50)
    {
        sf::Transformable::move(movement.x, speed*timer.asMilliseconds());
        
        return true;
    }
    else
        return false;
}


