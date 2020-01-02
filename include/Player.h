#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <SFML/Graphics.hpp>

using namespace std;

/**
Define these extern in your main.cpp
ex:
string defaulttexture1 = "filename1.jpeg";
string defaulttexture2 = "filename2.jpeg";
**/
extern string defaulttexture1;
extern string defaulttexture2;

/**
functions designed to be use at 60 fps
use window.setframerate(60); in main
**/

class Player
{
    int lives;
    float speed;
    float limit;
    bool moving; // used for animation refresh
    sf::RectangleShape* sprite;
    sf::Texture playertexture;
    sf::Texture playertexture2;
    sf::Clock ck; // used for animation refresh rate
    sf::Time time; // used for animation refresh rate
    sf::Clock frame; // used for framerate
    sf::Time rate; //used for framerate
public:
    Player(const sf::RenderWindow& window,string pt1 = defaulttexture1,string pt2 = defaulttexture2,float f1 = 200.0f,float f2 = 200.0f);
    ~Player();
    void change(const sf::Texture& text); //changes current sprite texture
    bool isAlive() const;
    int getlives();
    void changelives(int amount);
    sf::RectangleShape getsprite();
    void moveleft();
    void moveright();
    void moveup();
    void movedown();
    float carspeed(float spd = 0);
    float carspeed() const;
    void setlimit(float lt= .2f);
    float getlimit() const;
    bool update(); // updates player in window
    bool refresh(sf::RenderWindow& win); //refreshing player textures
    bool load(string pt1 = defaulttexture1,string pt2 = defaulttexture2);
};

#endif // PLAYER_H_INCLUDED
