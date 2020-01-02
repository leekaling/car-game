
#ifndef JayWalker_hpp
#define JayWalker_hpp

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Obstacle.h"
#include "Animation.h"
using namespace std;

class JayWalker: public Obstacle, public Animation
{
public:
    JayWalker(sf::Vector2f frogSize, sf::Texture& imageTexture, sf::Vector2u imageCount, float switchTime);
    ~JayWalker();
    bool move(float) override;
};

#endif /* JayWalker_hpp */
