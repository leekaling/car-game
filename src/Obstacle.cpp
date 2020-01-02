

#include "Obstacle.h"
using namespace std;


Obstacle::Obstacle(sf::Texture& imageTexture, sf::Vector2f obsSize)
: sf::RectangleShape(obsSize), speed(0.1), status(Visible), isExplode(false)
{
    
    position = sf::Vector2f(200.0f+(rand()%644),-obsSize.y/2.0f);
    setPosition(position);
    
    setOrigin(sf::Vector2f(obsSize.x/2.0f, obsSize.y/2.0f));
    
    setTexture(&imageTexture);
}

Obstacle::~Obstacle(){}

bool Obstacle::isVisible() const
{
    return status == Visible;
}

bool Obstacle::isHit()
{
    return status == Hit;
}

void Obstacle::setStatus(Status x)
{
    status = x;
}

void Obstacle::setSpeed(float x)
{
    speed = x;
}

float Obstacle::getSpeed()
{
    return speed;
}
