

#ifndef Box_hpp
#define Box_hpp

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Obstacle.h"
using namespace std;

class Box : public Obstacle
{
private:
    
public:
    Box(sf::Vector2f rockSize, sf::Texture& imageTexture);
    ~Box();
    bool move(float) override;
};

#endif /* Box_hpp */
