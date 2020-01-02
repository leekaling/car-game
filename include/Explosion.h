

#ifndef Explosion_hpp
#define Explosion_hpp

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "Obstacle.h"
#include "Animation.h"
using namespace std;

class Explosion : public Obstacle, public Animation
{

protected:
    bool isExplode;

public:
    Explosion(sf::Texture& texture, sf::Vector2f obsSize, sf::Vector2f obsPos, sf::Vector2u imageCount, float switchTime);
    ~Explosion(){}
    
    virtual bool move(float) override;
    
    bool getIsExp();
    void setIsExp(bool);

};


#endif /* Explosion_hpp */
