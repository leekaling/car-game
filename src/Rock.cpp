

#include <SFML/Graphics.hpp>
#include "Rock.h"

Rock::Rock(sf::Vector2f rockSize, sf::Texture& imageTexture)
: Obstacle(imageTexture, rockSize)
{
    cout << "Rock created." << endl;
    changeLives = -1;
    timer = sf::Time::Zero;
    obstacleId = rock;
}

Rock::~Rock()
{
    
}

bool Rock::move(float f)
{
    
    timer = ck.restart();
    
    sf::Transformable::move(0.0f,speed*timer.asMilliseconds());
    if (getPosition().y > 768 + 50) {
        return false;
    }
    return true;
}


