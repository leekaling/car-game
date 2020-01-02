

#ifndef Obstacles_hpp
#define Obstacles_hpp

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
using namespace std;

const sf::Vector2f InvaderSize{32.0f,32.0f};

const unsigned MainWindowWidth = 1024;
const unsigned MainWindowHeight = 768;

class Obstacle : public sf::RectangleShape
{
    
protected:
    
    enum Status {Visible,Hit,Invisible};
    enum obstacleType {rock,box,frog,jaywalker,explosion,deadbody};

    float speed;
    Status status;
    bool isExplode;
    
    int changeLives;
    obstacleType obstacleId;
    
    sf::Texture texture;
    sf::Vector2f position;
    
    sf::Texture explosionTexture;
    
    sf::Clock ck;
    sf::Time timer;
    
public:
    Obstacle(sf::Texture& imageTexture, sf::Vector2f obsSize);
    virtual ~Obstacle();
    virtual bool move(float) = 0;
    
    bool isVisible() const;
    bool isHit();
    void setStatus(Status x);
    void setSpeed(float x);
    float getSpeed();
    
    int getChangeLives() { return changeLives; }
    int getObstacleId() { return static_cast<int>(obstacleId); }
    // Rock = 1, Box = 2, Frog = 3, Walker = 4
};


#endif /* Obstacles_hpp */
