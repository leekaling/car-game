
#include "Box.h"

Box::Box(sf::Vector2f boxSize, sf::Texture& imageTexture)
: Obstacle(imageTexture, boxSize)
{
    cout << "Box created." << endl;
    changeLives = 1;
    timer = sf::Time::Zero;
    obstacleId = box;
}

Box::~Box()
{
    
}

bool Box::move(float f)
{
    
    timer = ck.restart();

    sf::Transformable::move(0.0f,speed*timer.asMilliseconds());
    if (getPosition().y > 768 + 50){
        return false;
    }
    return true;
}
