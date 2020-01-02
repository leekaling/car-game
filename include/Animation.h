

#ifndef Animation_hpp
#define Animation_hpp

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Obstacle.h"

class Animation
{
protected:
    sf::Vector2u animImageCount;    // amount of images in each row, column
    sf::Vector2u currentImage;  // current position of the texture
    sf::IntRect uvRect;
    float switchTime;
    float totalTime;
    float horizontalSpeed;
    
    bool faceRight;
    bool faceDown;
    sf::Vector2f movement;
    int row;
    
public:
    Animation(const sf::Texture* animTexture, sf::Vector2u imageCount, float switchTime);
    ~Animation();
    virtual sf::IntRect update_uvRect(float deltaTime);
    float getHorizontalSpeed();
};

#endif /* Animation_hpp */
