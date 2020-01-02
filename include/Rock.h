

#ifndef Rock_hpp
#define Rock_hpp

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Obstacle.h"

using namespace std;

class Rock : public Obstacle
{
private:

    
public:
    Rock(sf::Vector2f rockSize, sf::Texture& imageTexture);
    ~Rock();
    bool move(float) override;
};

#endif /* Rock_hpp */
