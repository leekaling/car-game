
#include <SFML/Graphics.hpp>
#include "DeadBody.h"

DeadBody::DeadBody(sf::Vector2f rockSize, sf::Texture& imageTexture, sf::Vector2f pos)
: Obstacle(imageTexture, rockSize)
{
    cout << "DeadBody created." << endl;
    changeLives = 0;
    timer = sf::Time::Zero;
    setPosition(pos);
    obstacleId = deadbody;
}

DeadBody::~DeadBody(){}

bool DeadBody::move(float f)
{
    
    timer = ck.restart();

    sf::Transformable::move(0.0f,speed*timer.asMilliseconds());
    if (getPosition().y > 768 + 50) {
        return false;
    }
    return true;
}

