#include <iostream>
#include "Player.h"

Player::Player(const sf::RenderWindow& window,string pt1,string pt2,float f1,float f2)
{
    try
    {
        if((f1 < 0) || (f2 < 0))
            throw(range_error("wrong float coordinates"));
        lives = 5;
        speed = .2f;
        limit = .2f;
        moving = false;
        if(!load(pt1.c_str(),pt2.c_str()))
            throw(invalid_argument("bad texture files"));
        time = sf::Time::Zero;
        rate = sf::Time::Zero;
        sprite = new sf::RectangleShape(sf::Vector2f(f1,f2));
        sprite->setOrigin(sprite->getSize().x/2,sprite->getSize().y);
        sprite->setPosition(window.getSize().x/2,window.getSize().y*(19.0/20.0));
        change(playertexture);
    }
    catch(range_error e)
    {
        cerr << "Cannot create player because: " << e.what() << endl;
    }
    catch(invalid_argument e)
    {
        cerr << "Cannot create player because: " << e.what() << endl;
    }
}

Player::~Player()
{
    delete sprite;
}

void Player::change(const sf::Texture& text)
{
    sprite->setTexture(&text);
}

bool Player::isAlive() const
{
    return (lives != 0);
}

int Player::getlives()
{
    return lives;
}

void Player::changelives(int amount)
{
    int r = lives + amount;
    
    if (lives == 0) {
        lives = 0;
    }
    else if (r <= 0) {
        lives = 0;
    }
    else if (r > 5) {
        lives = 5;
    }
    else {
        lives = r;
    }
}

sf::RectangleShape Player::getsprite()
{
    return *sprite;
}

void Player::moveleft()
{
    sprite->move(0-(speed * rate.asMilliseconds()),0.0f);
}

void Player::moveright()
{
    sprite->move((speed * rate.asMilliseconds()),0.0f);
}

void Player::moveup()
{
    sprite->move(0.0f,0-(speed * rate.asMilliseconds()));
}

void Player::movedown()
{
    sprite->move(0.0f,(speed * rate.asMilliseconds()));
}

float Player::carspeed(float spd)
{
    if((speed+spd <= limit) && (speed+spd >= 0))
    {
        speed += spd;
    }
    else if(speed+spd > limit)
    {
        speed = limit;
    }
    else if(speed+spd < 0)
    {
        speed = 0;
    }
    return speed;
}

float Player::carspeed() const
{
    return speed;
}

void Player::setlimit(float lt)
{
    limit = lt;
}

float Player::getlimit() const
{
    return limit;
}

bool Player::update()
{
    if(isAlive())
    {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        {
            if(sprite->getPosition().x >= 220.0f)
                moveleft();
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        {
            if(sprite->getPosition().x <= 824.0f)
                moveright();
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        {
            //moveup();
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        {
            //movedown();
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
        {
            //carspeed(.0005);
        }
        return true;
    }
    return false;
}

bool Player::refresh(sf::RenderWindow& win)
{
    time += ck.restart();
    rate = frame.restart();
    if (time >= sf::seconds(20.0f / 60.0f)) // 3 fps
    {
        time = sf::Time::Zero;
        if (moving)
        {
            moving = false;
            change(playertexture);
        }
        else
        {
            moving = true;
            change(playertexture2);
        }
        win.draw(getsprite());
        return true;
    }
    win.draw(getsprite());
    return false;
}

bool Player::load(string pt1,string pt2)
{
    try
    {
        if(!playertexture.loadFromFile(pt1.c_str()))
            throw(invalid_argument("cannot open texture 1"));
        if(!playertexture2.loadFromFile(pt2.c_str()))
            throw(invalid_argument("cannot open texture 2"));
        return true;
    }
    catch(invalid_argument e)
    {
        cerr << "Unable to load because: " << e.what() << endl;
        return false;
    }
}
