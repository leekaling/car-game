
#include <SFML/Graphics.hpp>
#include "Frog.h"

Frog::Frog(sf::Vector2f frogSize, sf::Texture& imageTexture, sf::Vector2u imageCount, float switchTime)
: Obstacle(imageTexture, frogSize), Animation(getTexture(), imageCount, switchTime)
{
    cout << "Frog created." << endl;
    changeLives = -1;
    timer = sf::Time::Zero;
    obstacleId = frog;
}

Frog::~Frog(){}

bool Frog::move(float deltaTime)
{

    timer = ck.restart();
    
    movement = sf::Vector2f(0.0f, 0.0f);
    sf::Vector2f pos = getPosition();
    
    setTextureRect(update_uvRect(deltaTime));
    
    if (pos.y < 768 + 50) {
        
        if ( pos.x < 676.0f && pos.x > 350.0f)
        {
            if (faceRight)
                pos.x += 0.1f ;
            else
                pos.x -= 0.1f ;
            //pos.y += 0.1f ;
        }
        else
        {
            if (pos.x < 500)
            {
                faceRight = true;
                pos.x += 0.1f ;
            }
            else
            {
                faceRight = false;
                pos.x -= 0.1f ;
            }
        }
        
        pos.x += movement.x;
        setPosition(pos);
        sf::Transformable::move(movement.x, speed*timer.asMilliseconds());
        
        return true;
    }
    else
        return false;
}
