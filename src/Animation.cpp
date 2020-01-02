

#include "Animation.h"

Animation::Animation(const sf::Texture* texture, sf::Vector2u imageCount, float switchTime)
:animImageCount(imageCount), switchTime(switchTime), totalTime(0.0f), horizontalSpeed(0.25f),
 faceRight(rand()%2), faceDown(rand()%2), movement(0.0, 0.0f), row(0)
{
    currentImage.x = 0;
    uvRect.width  = texture->getSize().x / static_cast<float>(animImageCount.x);
    uvRect.height = texture->getSize().y / static_cast<float>(animImageCount.y);
}

Animation::~Animation(){}

sf::IntRect Animation::update_uvRect(float deltaTime)
{
    currentImage.y = row;
    totalTime += deltaTime;

    if (totalTime >= switchTime)
    {
        totalTime -= switchTime;
        currentImage.x++;
        if (currentImage.x > animImageCount.x-1)
            currentImage.x = 0;
    }
    uvRect.top  = currentImage.y * uvRect.height;

    if (faceRight)
    {
        uvRect.left = currentImage.x * uvRect.width;
        uvRect.width = abs(uvRect.width);
    }
    else
    {
        uvRect.left = (currentImage.x + 1) * abs(uvRect.width);
        uvRect.width = -abs(uvRect.width);
    }
    return uvRect;
}

float Animation::getHorizontalSpeed()
{
    return horizontalSpeed;
}
