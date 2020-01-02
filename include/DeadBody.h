

#ifndef DeadBody_hpp
#define DeadBody_hpp

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Obstacle.h"

using namespace std;

class DeadBody : public Obstacle
{
private:
    
public:
    DeadBody(sf::Vector2f rockSize, sf::Texture& imageTexture, sf::Vector2f pos);
    ~DeadBody();
    bool move(float) override;
};

#endif /* DeadBody_hpp */
