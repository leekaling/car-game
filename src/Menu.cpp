//
//  Menu.cpp
//  CarGame
//
//  Created by Ethan Huang on 3/18/19.
//  Copyright © 2019 Ethan Huang. All rights reserved.
//

#include "Menu.h"
#include "Resources.h"

using namespace FontFiles;

Menu::Menu(sf::RenderWindow& window, string s1, string s2, string s3)
{
    if (!font.loadFromFile(mainFont))
        std::cout << "Can't find font" << endl;
    
    title.setFont(font);
    title.setString("DANGEROUS DRIVER");
    title.setCharacterSize(80);
    title.setFillColor(sf::Color(232,112,27));
    title.setOutlineColor(sf::Color::Black);
    title.setOutlineThickness(10.0f);
    title.setStyle(sf::Text::Bold);
    title.setOrigin(title.getLocalBounds().left + title.getLocalBounds().width / 2.0f, title.getLocalBounds().top + title.getLocalBounds().height / 2.0f);
    title.setPosition(1024/2.0f, 200);
    
    item[0].setFont(font);
    item[0].setString(s1);
    item[0].setCharacterSize(50);
    item[0].setFillColor(sf::Color::Yellow);
    item[0].setOutlineColor(sf::Color::Black);
    item[0].setOutlineThickness(7.0f);
    item[0].setStyle(sf::Text::Bold);
    item[0].setOrigin(item[0].getLocalBounds().left + item[0].getLocalBounds().width / 2.0f, item[0].getLocalBounds().top + item[0].getLocalBounds().height / 2.0f);
    item[0].setPosition(1024/2.0f, 400);
    
    item[1].setFont(font);
    item[1].setString(s2);
    item[1].setCharacterSize(50);
    item[1].setFillColor(sf::Color::Yellow);
    item[1].setOutlineColor(sf::Color::Black);
    item[1].setOutlineThickness(7.0f);
    item[1].setStyle(sf::Text::Bold);
    item[1].setOrigin(item[1].getLocalBounds().left + item[1].getLocalBounds().width / 2.0f, item[1].getLocalBounds().top + item[1].getLocalBounds().height / 2.0f);
    item[1].setPosition(1024/2.0f, 500);
    
    item[2].setFont(font);
    item[2].setString(s3);
    item[2].setCharacterSize(50);
    item[2].setFillColor(sf::Color::Yellow);
    item[2].setOutlineColor(sf::Color::Black);
    item[2].setOutlineThickness(7.0f);
    item[2].setStyle(sf::Text::Bold);
    item[2].setOrigin(item[2].getLocalBounds().left + item[2].getLocalBounds().width / 2.0f, item[2].getLocalBounds().top + item[2].getLocalBounds().height / 2.0f);
    item[2].setPosition(1024/2.0f, 600);
    
    selectedItem = 1;
}
void Menu::draw(sf::RenderWindow &window)
{
    for (int i=0; i<3; i++) {
        window.draw(item[i]);
    }
}
void Menu::moveUp()
{
    if (selectedItem >= 1 )
    {
        item[selectedItem].setFillColor(sf::Color::Yellow);
        item[--selectedItem].setFillColor(sf::Color::Red);
    }
}
void Menu::moveDown()
{
    if (selectedItem+1 <= 2)
    {
        item[selectedItem].setFillColor(sf::Color::Yellow);
        item[++selectedItem].setFillColor(sf::Color::Red);
    }
}
