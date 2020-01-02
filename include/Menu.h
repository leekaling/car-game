//
//  Menu.hpp
//  CarGame
//
//  Created by Ethan Huang on 3/18/19.
//  Copyright © 2019 Ethan Huang. All rights reserved.
//

#ifndef Menu_hpp
#define Menu_hpp

#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;

class Menu
{
private:
    int selectedItem;
    sf::Font font;
public:
    sf::Text item[3];
    sf::Text title;
public:
    Menu(sf::RenderWindow& window, string, string, string);
    void draw(sf::RenderWindow& window);
    void moveUp();
    void moveDown();
    int getSelectedItem(){return selectedItem;}
    void setSelectedItem(int i){selectedItem=i;}
};

#endif /* Menu_hpp */
