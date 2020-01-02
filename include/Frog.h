

#ifndef Frog_hpp
#define Frog_hpp

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Obstacle.h"
#include "Animation.h"
using namespace std;

class Frog: public Obstacle, public Animation
{
private:

public:
    Frog(sf::Vector2f frogSize, sf::Texture& imageTexture, sf::Vector2u imageCount, float switchTime);
    ~Frog ();
    bool move(float) override;
};


#endif // frog.h
