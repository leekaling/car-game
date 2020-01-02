

#include "Explosion.h"

Explosion::Explosion(sf::Texture& texture, sf::Vector2f obsSize, sf::Vector2f obsPos, sf::Vector2u imageCount, float switchTime)
: Obstacle(texture, obsSize), Animation(&texture, imageCount, switchTime), isExplode(false)
{
    cout << "explosion created." << endl;
    setOrigin(sf::Vector2f(obsSize.x/2.0f, obsSize.y/2.0f));
    setPosition(obsPos);
    setTexture(&texture);
    changeLives = 0;
    timer = sf::Time::Zero;
    obstacleId = explosion;
}

bool Explosion::move(float deltaTime)
{
    // update texture
    currentImage.y = row;
    totalTime += deltaTime;
    if (totalTime >= switchTime)
    {
        totalTime -= switchTime;
        currentImage.x++;
        if (currentImage.x > animImageCount.x-1)
        {
            currentImage.x = 0;
            return false;
        }
    }
    uvRect.top  = currentImage.y * uvRect.height;
    uvRect.left = currentImage.x * uvRect.width;
    
    setTextureRect(uvRect);

    sf::Transformable::move(0.0f,speed*timer.asMilliseconds());
    if (getPosition().y > 768 + 50){
        return false;
    }
    
    else
        return true;
}

bool Explosion::getIsExp(){return isExplode;}

void Explosion::setIsExp(bool x){isExplode = x;}



