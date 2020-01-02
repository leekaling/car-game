#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <list>
#include <set>
#include <vector>
#include "Menu.h"
#include "Obstacle.h"
#include "Animation.h"
#include "Rock.h"
#include "Box.h"
#include "Frog.h"
#include "JayWalker.h"
#include "Player.h"
#include "Explosion.h"
#include "DeadBody.h"
#include "Score.h"
#include "Resources.h"

using namespace std;
using namespace ImageFiles;
using namespace SoundFiles;
using namespace FontFiles;
using namespace TextFiles;

string defaulttexture1 = filePrefix + "carz.png";
string defaulttexture2 = filePrefix + "carz2.png";

float volume;

sf::Vector2f ObstacleSize{80, 80};
sf::Vector2u frogImageCount = sf::Vector2u(8, 1);
sf::Vector2u walkerImageCount = sf::Vector2u(3, 4);
sf::Vector2u explosionImageCount = sf::Vector2u(15, 1);

multiset<Score, greater<Score>> scoreboard;

void readScoreboard();
void writeNewScoreToFile(Score newScore);
void playGame(sf::RenderWindow& window);
void endGame(sf::RenderWindow& window, bool win, int opacity);
string enterName(sf::RenderWindow& window);
void displayMenu(sf::RenderWindow& window);
void displayStory(sf::RenderWindow&);
void displayInstruction(sf::RenderWindow&);
void displayResult(sf::RenderWindow& window, int finalScore);
void displayScoreboard(sf::RenderWindow& window);
void moveObstacles(sf::RenderWindow& win, list<Obstacle*>& obstacleList, bool speedChanged, float deltaTime);
void road(sf::RenderWindow& win, float speed);
int calculateScore(sf::RenderWindow& win, int level, int timeInHundredth, int currentScore);
void showScore(sf::RenderWindow& win, int level, int finalScore);
void lives(sf::RenderWindow& win, int lives);

int main()
{
    srand(time(0));
    
    sf::RenderWindow window(sf::VideoMode(1024, 768), gameName);
    displayMenu(window);

    window.clear();
    return 0;
}

void readScoreboard()
{
    ifstream fin(scoreboardText);
    if (!fin)
    {
        cerr << "The file is not found.\n";
        exit(-1);
    }
    
    string temp1;
    string temp2;
    int scoreTemp;
    getline(fin,temp1);
    while (getline(fin,temp1,',') && !fin.eof())
    {
        getline(fin,temp2);
        
        scoreTemp = stoi(temp2);
        Score temp(temp1, scoreTemp);
        scoreboard.insert(temp);
    }
    fin.close();
}

void writeNewScoreToFile(Score newScore)
{
    ofstream fout(scoreboardText,fstream::app);
    fout << newScore << endl;
    fout.close();
}

void playGame(sf::RenderWindow& window)
{
    string playerName = enterName(window);
    
    // Score calculation
    int level = 1;
    int timeInHundredth = 0;
    int currentScore = 0;
    int finalScore = 0;
    
    // Game Speed controls
    float deltaTime = 0.0f;
    float switchTime = 0.2f;
    sf::Clock clock;
    
    sf::Clock speedControlClock;
    sf::Time totalTime = speedControlClock.restart();
    float speed = 0.2f;
    bool speedChanged = false;
    
    unsigned loopCount = 0;
    unsigned opacity = 0;
    
    bool endFlag = false;
    bool win = true;
    
    // Player
    Player pl(window);
    
    // Obstacle textures and soundbuffers
    sf::Texture RockTexture; RockTexture.loadFromFile(rockImage);
    sf::Texture BoxTexture; BoxTexture.loadFromFile(boxImage);
    sf::Texture FrogTexture; FrogTexture.loadFromFile(frogImage);
    sf::Texture WalkerTexture; WalkerTexture.loadFromFile(walkerImage);
    sf::Texture DeadBodyTexture; DeadBodyTexture.loadFromFile(deadBodyImage);
    sf::Texture explosionTexture; explosionTexture.loadFromFile(explosionImage);
    
    sf::SoundBuffer ExplosionSoundBuffer; ExplosionSoundBuffer.loadFromFile(explosionSound);
    sf::SoundBuffer boxExplosionSoundBuffer; boxExplosionSoundBuffer.loadFromFile(boxExplosionSound);
    sf::SoundBuffer walkerExplosionSoundBuffer; walkerExplosionSoundBuffer.loadFromFile(walkerExplosionSound);
    
    // list of obstacle pointers, use of polymorphism
    list<Obstacle*> obstacleList;
    
    // music
    static sf::Music backgroundMusic;
    static sf::Music gameoverMusic;
    static sf::Music winMusic;
    static bool loadMusic = true;
    if (loadMusic)
    {
        backgroundMusic.openFromFile(backgroundSound);
        backgroundMusic.setVolume(volume);
        backgroundMusic.setLoop(true);
        gameoverMusic.openFromFile(gameoverSound);
        winMusic.openFromFile(winSound);
        loadMusic = false;
    }

    volume = 0.0f;
    
    backgroundMusic.stop();
    backgroundMusic.play();
    
    sf::Sound explosionSound;
    explosionSound.setBuffer(ExplosionSoundBuffer);
    explosionSound.setVolume(20);
    
    sf::Sound boxExplosionSound;
    boxExplosionSound.setBuffer(boxExplosionSoundBuffer);
    boxExplosionSound.setVolume(20);
    
    sf::Sound walkerExplosionSound;
    walkerExplosionSound.setBuffer(walkerExplosionSoundBuffer);
    walkerExplosionSound.setVolume(20);
    
    // game loop
    while (window.isOpen())
    {
        // volume control
        if (volume < 40.0f && !endFlag)
        {
            volume += 0.05f;
            backgroundMusic.setVolume(volume);
        }
        
        // speed control
        deltaTime = clock.restart().asSeconds();
        totalTime = speedControlClock.getElapsedTime();
        if(timeInHundredth >= 5750) { }
        else
        {
            timeInHundredth = (int)totalTime.asMilliseconds() / 10;
            if (timeInHundredth / level == 500 && level < 10)
            {
                currentScore += level*level*25*10;
                level++;
                speed += 0.1f;
                speedControlClock.restart();
                speedChanged = true;
            }
        }
        
        // create random obstacles
        if (loopCount > 250 - (speed*200) && !endFlag )
        {
            Obstacle* obstaclePtr = nullptr;
            switch (rand()%9){
                case 0: case 1:
                    obstaclePtr = new Box(ObstacleSize, BoxTexture);
                    break;
                case 2: case 3: case 4:
                    obstaclePtr = new Rock(ObstacleSize, RockTexture);
                    break;
                case 5: case 6:
                    obstaclePtr = new Frog(ObstacleSize, FrogTexture, frogImageCount, switchTime);
                    break;
                case 7: case 8:
                    obstaclePtr = new JayWalker(ObstacleSize, WalkerTexture, walkerImageCount, switchTime);
                    break;
                default:
                    break;
            }
            obstaclePtr->setSpeed(speed);
            obstacleList.push_back(obstaclePtr);
            loopCount = 0;
        }
        
        // event loop
        sf::Event event;
        while (window.pollEvent(event)){
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Space) { }
                    else if (event.key.code == sf::Keyboard::N) // back door to go to next stage
                    {
                        if (level == 10) { timeInHundredth = 5750;}
                        else if(timeInHundredth >= 5750) { }
                        else if (level < 10)
                        {
                            currentScore += level*level*25*10;
                            level++;
                            speed += 0.1f;
                            speedControlClock.restart();
                            speedChanged = true;
                        }
                    }
                    else if (event.key.code == sf::Keyboard::Escape)
                        window.close();
                default:;
            }
        }
        
        // moving road
        road(window,speed);
        
        // move obstacles
        moveObstacles(window,obstacleList,speedChanged,deltaTime);
        speedChanged = false;
        
        // player
        pl.update();
        pl.refresh(window);
        
        // check collision
        for ( auto it = obstacleList.begin(); it != obstacleList.end(); ++it)
        {
            sf::FloatRect playerRect(pl.getsprite().getGlobalBounds().left+70.0f, pl.getsprite().getGlobalBounds().top+70.0f, pl.getsprite().getGlobalBounds().width-140.0f, pl.getsprite().getGlobalBounds().height-140.0f);
            if ((*it)->getChangeLives()!=0 && playerRect.intersects((*it)->getGlobalBounds()))
            {
                cout << "*collided*" << endl;
                pl.changelives((*it)->getChangeLives());
                Obstacle *temp = nullptr;
                int obstacleId = (*it)->getObstacleId();
                switch (obstacleId)
                {
                    case 0: case 2:
                        temp = new Explosion(explosionTexture, ObstacleSize, (*it)->getPosition(), explosionImageCount, switchTime);
                        temp->setSpeed(speed);
                        explosionSound.stop();
                        explosionSound.play();
                        obstacleList.erase(it);
                        obstacleList.push_back(temp);
                        break;
                    case 1:
                        boxExplosionSound.stop();
                        boxExplosionSound.play();
                        obstacleList.erase(it);
                        break;
                    case 3:
                        temp = new DeadBody(ObstacleSize, DeadBodyTexture, (*it)->getPosition());
                        temp->setSpeed(speed);
                        walkerExplosionSound.stop();
                        walkerExplosionSound.play();
                        obstacleList.erase(it);
                        obstacleList.push_back(temp);
                        break;
                    default:
                        ;
                    
                        
                }
            }
        }
        
        // calculate score
        if(!endFlag)
        {
            finalScore = calculateScore(window,level,timeInHundredth,currentScore);
        }
        
        // show score
        showScore(window,level,finalScore);
        
        //show lives
        lives(window,pl.getlives());
        if (pl.getlives()==0)
        {
            endFlag = true;
            win = false;
            if (gameoverMusic.getStatus() != sf::Music::Playing)
            {
                gameoverMusic.stop();
                gameoverMusic.play();
            }
        }
        if (finalScore == 100000)
        {
            endFlag = true;
            if (winMusic.getStatus() != sf::Music::Playing)
            {
                winMusic.stop();
                winMusic.play();
            }
        }
        
        if (endFlag && opacity < 255)
        {
            endGame(window, win, opacity);
            opacity++;
        }
        if (opacity == 255)
        {
            endGame(window, win, opacity);
            if (speed>0.05) speed -= 0.0001f;
        }
        
        if (endFlag)
        {
            backgroundMusic.stop();
            
            sf::Event event2;
            while (window.pollEvent(event2)){
                switch (event2.type) {
                    case sf::Event::Closed:
                        window.close();
                        break;
                    case sf::Event::KeyPressed:
                        if (event2.key.code == sf::Keyboard::Space)
                        {
                            // write new score to file
                            Score newScore(playerName,finalScore);
                            writeNewScoreToFile(newScore);
                            
                            window.clear();
                            displayResult(window,finalScore);
                            break;
                        }
                        else if (event2.key.code == sf::Keyboard::Escape)
                            window.close();
                    default:;
                }
            }
        }
        
        window.display();
        loopCount++;
    }
}

void endGame(sf::RenderWindow& window, bool win, int opacity)
{
    sf::Font font;
    if (!font.loadFromFile(endFont))
        std::cout << "Can't find font" << std::endl;
    
    sf::Text message;
    message.setFont(font);
    if (win) message.setString("YOU WIN!");
    else message.setString("GAME OVER");
    message.setCharacterSize(100);
    message.setFillColor(sf::Color(255, 255, 255, opacity));
    message.setStyle(sf::Text::Bold);
    message.setOrigin(message.getLocalBounds().left + message.getLocalBounds().width / 2.0f, message.getLocalBounds().top + message.getLocalBounds().height / 2.0f);
    message.setPosition(1024/2.0f,768/2.0f);
    
    sf::Text cont;
    cont.setFont(font);
    cont.setString("Press Space to Continue...");
    cont.setCharacterSize(24);
    cont.setFillColor(sf::Color(255, 255, 255, opacity));
    cont.setStyle(sf::Text::Bold);
    cont.setOrigin(cont.getLocalBounds().left + cont.getLocalBounds().width / 2.0f, cont.getLocalBounds().top + cont.getLocalBounds().height / 2.0f);
    cont.setPosition(1024/2.0f,470);
    
    window.draw(message);
    window.draw(cont);
}

void displayMenu(sf::RenderWindow& window)
{
    sf::Texture backgroundTexture;
    sf::Sprite background;
    
    backgroundTexture.loadFromFile(roadImage);
    background.setTexture(backgroundTexture);
    background.setScale(1.22,1.18);
    background.setTextureRect(sf::IntRect(0, 0, 1024, 768));
    background.setPosition(0.0f,0.0f);
    background.setColor(sf::Color(255, 255, 255, 198));
    
    Menu menu(window, "START", "HOW TO PLAY", "SCOREBOARD");
    
    static sf::Music mainMusic;
    static sf::Music swoosh;
    static bool loadMusic = true;
    if (loadMusic)
    {
        mainMusic.openFromFile(mainSound);
        swoosh.openFromFile(swooshSound);
        mainMusic.setLoop(true);
        loadMusic = false;
    }
    if (mainMusic.getStatus() != sf::Music::Playing)
    {
        mainMusic.stop();
        mainMusic.play();
    }
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition( window );
            sf::Vector2f mousePosF( static_cast<float>( mousePos.x ), static_cast<float>( mousePos.y ) );
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseMoved:
                {
                    for (int i=0; i<3; i++)
                    {
                        if (menu.item[i].getGlobalBounds().contains( mousePosF ))
                        {
                            menu.item[i].setFillColor(sf::Color::Red);
                            if (i>0)
                                menu.setSelectedItem(i);
                        }
                        else
                            menu.item[i].setFillColor(sf::Color::Yellow);
                    }
                }
                    break;
                case sf::Event::MouseButtonPressed:
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition( window );
                    sf::Vector2f mousePosF( static_cast<float>( mousePos.x ), static_cast<float>( mousePos.y ) );
                    for (int i=0; i<3; i++) {
                        if (menu.item[i].getGlobalBounds().contains( mousePosF ))
                        {
                            if (menu.item[i].getGlobalBounds().contains( mousePosF ))
                                menu.item[i].setFillColor(sf::Color::White);
                            else
                                menu.item[i].setFillColor(sf::Color::Yellow);
                        }
                    }
                }
                    break;
                case sf::Event::MouseButtonReleased:
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition( window );
                    sf::Vector2f mousePosF( static_cast<float>( mousePos.x ), static_cast<float>( mousePos.y ) );
                    if (menu.item[0].getGlobalBounds().contains( mousePosF ))
                    {
                        mainMusic.stop();
                        swoosh.stop();
                        swoosh.play();
                        menu.item[0].setFillColor(sf::Color::Red);
                        window.clear();
                        playGame(window);
                    }
                    if (menu.item[1].getGlobalBounds().contains( mousePosF ))
                    {
                        swoosh.stop();
                        swoosh.play();
                        menu.item[1].setFillColor(sf::Color::Red);
                        window.clear();
                        displayStory(window);
                    }
                    if (menu.item[2].getGlobalBounds().contains( mousePosF ))
                    {
                        swoosh.stop();
                        swoosh.play();
                        menu.item[2].setFillColor(sf::Color::Red);
                        window.clear();
                        displayScoreboard(window);
                    }
                }
                    break;
                    
                case sf::Event::KeyPressed:
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                    {
                        menu.moveUp();
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                    {
                        menu.moveDown();
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                    {
                        window.close();
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
                    {
                        switch (menu.getSelectedItem())
                        {
                            case 0:
                                swoosh.stop();
                                swoosh.play();
                                mainMusic.stop();
                                window.clear();
                                playGame(window);
                            case 1:
                                swoosh.stop();
                                swoosh.play();
                                window.clear();
                                displayStory(window);
                            case 2:
                                swoosh.stop();
                                swoosh.play();
                                window.clear();
                                displayScoreboard(window);
                            default:;
                        }
                    }
                default:;
            }
            
        }
        window.clear(sf::Color(237,239,203));
        window.draw(background);
        window.draw(menu.title);
        menu.draw(window);
        window.display();
    }
    
}

string enterName(sf::RenderWindow& window)
{
    sf::Texture backgroundTexture;
    sf::Sprite background;
    
    backgroundTexture.loadFromFile(roadImage);
    background.setTexture(backgroundTexture);
    background.setScale(1.22,1.18);
    background.setTextureRect(sf::IntRect(0, 0, 1024, 768));
    background.setPosition(0.0f,0.0f);
    background.setColor(sf::Color(255, 255, 255, 198));
    
    sf::Font font;
    if (!font.loadFromFile(mainFont))
        std::cout << "Can't find font" << std::endl;
    
    sf::Font font2;
    if (!font2.loadFromFile(nameFont))
        std::cout << "Can't find font" << std::endl;
    
    sf::Text text;
    text.setFont(font);
    text.setString("Enter your name...");
    text.setCharacterSize(60);
    text.setFillColor(sf::Color(232,146,79));
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(10.0f);
    text.setStyle(sf::Text::Bold);
    text.setOrigin(text.getLocalBounds().left + text.getLocalBounds().width / 2.0f, text.getLocalBounds().top + text.getLocalBounds().height / 2.0f);
    text.setPosition(1024/2.0f,250);
    
    string playerInput = "";
    sf::Text playerText;
    playerText.setFont(font2);
    playerText.setCharacterSize(70);
    playerText.setFillColor(sf::Color::Yellow);
    playerText.setOutlineColor(sf::Color::Black);
    playerText.setOutlineThickness(10.0f);
    playerText.setStyle(sf::Text::Bold);
    playerText.setOrigin(playerText.getLocalBounds().left + playerText.getLocalBounds().width / 2.0f, playerText.getLocalBounds().top + playerText.getLocalBounds().height / 2.0f);
    playerText.setPosition(1024/2.0f,768.0f/2.0f);
    
    static sf::Music swoosh;
    static bool loadMusic = true;
    if (loadMusic)
    {
        swoosh.openFromFile(swooshSound);
        loadMusic = false;
    }
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::TextEntered:
                    if (playerInput.size() != 0 && event.text.unicode == '\b')
                    {
                        playerInput.erase(playerInput.size() - 1, 1);
                        playerText.setString(playerInput);
                        playerText.setOrigin(playerText.getLocalBounds().left + playerText.getLocalBounds().width / 2.0f, playerText.getLocalBounds().top + playerText.getLocalBounds().height / 2.0f);
                    }
                    if( (isalpha(event.text.unicode) || isdigit(event.text.unicode)) && playerInput.size() < 16)
                    {
                        playerInput += toupper(event.text.unicode);
                        playerText.setString(playerInput);
                        playerText.setOrigin(playerText.getLocalBounds().left + playerText.getLocalBounds().width / 2.0f, playerText.getLocalBounds().top + playerText.getLocalBounds().height / 2.0f);
                    }
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Enter)
                    {
                        swoosh.stop();
                        swoosh.play();
                        window.clear();
                        return playerInput;
                    }
                    if (event.key.code == sf::Keyboard::Escape)
                    {
                        window.close();
                    }
                default:
                    ;
            }
        }
        window.clear(sf::Color(237,239,203));
        window.draw(background);
        window.draw(text);
        window.draw(playerText);

        window.display();
    }
    return "";
}

void displayStory (sf::RenderWindow& window)
{
    sf::Texture backgroundTexture;
    sf::Sprite background;
    
    backgroundTexture.loadFromFile(roadImage);
    background.setTexture(backgroundTexture);
    background.setScale(1.22,1.18);
    background.setTextureRect(sf::IntRect(0, 0, 1024, 768));
    background.setPosition(0.0f,0.0f);
    background.setColor(sf::Color(255, 255, 255, 198));
    
    sf::Font font;
    if (!font.loadFromFile(storyFont))
        std::cout << "Can't find font" << std::endl;
    
    sf::Font font2;
    if (!font2.loadFromFile(mainFont))
        std::cout << "Can't find font" << std::endl;
    
    sf::Text sent1;
    sent1.setFont(font);
    sent1.setString("Hello, I am the shop owner, John.");
    sent1.setCharacterSize(37);
    sent1.setFillColor(sf::Color(232,146,79));
    sent1.setOutlineColor(sf::Color::Black);
    sent1.setOutlineThickness(7.0f);
    sent1.setStyle(sf::Text::Bold);
    sent1.setPosition(30.0f,80);
    
    sf::Text sent2;
    sent2.setFont(font);
    sent2.setString("I have to send a few packages to another town.");
    sent2.setCharacterSize(37);
    sent2.setFillColor(sf::Color(232,146,79));
    sent2.setOutlineColor(sf::Color::Black);
    sent2.setOutlineThickness(7.0f);
    sent2.setStyle(sf::Text::Bold);
    sent2.setPosition(30.0f,180);
    
    sf::Text sent3;
    sent3.setFont(font);
    sent3.setString("Unfortunately, the driver is sick today.");
    sent3.setCharacterSize(37);
    sent3.setFillColor(sf::Color(232,146,79));
    sent3.setOutlineColor(sf::Color::Black);
    sent3.setOutlineThickness(7.0f);
    sent3.setStyle(sf::Text::Bold);
    sent3.setPosition(30.0f,280);
    
    sf::Text sent4;
    sent4.setFont(font);
    sent4.setString("Can you help me deliver these packages?");
    sent4.setCharacterSize(37);
    sent4.setFillColor(sf::Color(232,146,79));
    sent4.setOutlineColor(sf::Color::Black);
    sent4.setOutlineThickness(7.0f);
    sent4.setStyle(sf::Text::Bold);
    sent4.setPosition(30.0f,380);
    
    sf::Text sent5;
    sent5.setFont(font);
    sent5.setString("However, be aware of the obstacles on your way...");
    sent5.setCharacterSize(37);
    sent5.setFillColor(sf::Color(232,146,79));
    sent5.setOutlineColor(sf::Color::Black);
    sent5.setOutlineThickness(7.0f);
    sent5.setStyle(sf::Text::Bold);
    sent5.setPosition(30.0f,530);
    
    sf::Text cont;
    cont.setFont(font2);
    cont.setString("Click to continue...");
    cont.setCharacterSize(35);
    cont.setFillColor(sf::Color::Yellow);
    cont.setOutlineColor(sf::Color::Black);
    cont.setOutlineThickness(7.0f);
    cont.setStyle(sf::Text::Bold);
    
    cont.setOrigin(cont.getLocalBounds().left + cont.getLocalBounds().width, cont.getLocalBounds().top + cont.getLocalBounds().height);
    cont.setPosition(924.0f,720.0f);
    
    static sf::Music swoosh;
    static bool loadMusic = true;
    if (loadMusic)
    {
        swoosh.openFromFile(swooshSound);
        loadMusic = false;
    }
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition( window );
            sf::Vector2f mousePosF( static_cast<float>( mousePos.x ), static_cast<float>( mousePos.y ) );
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseMoved:
                    if (cont.getGlobalBounds().contains( mousePosF ))
                        cont.setFillColor(sf::Color::Red);
                    else
                        cont.setFillColor(sf::Color::Yellow);
                case sf::Event::MouseButtonPressed:
                    if (cont.getGlobalBounds().contains( mousePosF ))
                        cont.setFillColor(sf::Color::Red);
                    else
                        cont.setFillColor(sf::Color::Yellow);
                    break;
                case sf::Event::MouseButtonReleased:
                    if (cont.getGlobalBounds().contains( mousePosF ))
                    {
                        swoosh.stop();
                        swoosh.play();
                        cont.setFillColor(sf::Color::Red);
                        window.clear();
                        displayInstruction(window);
                    }
                case sf::Event::KeyPressed:
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                    {
                        if (cont.getFillColor()==sf::Color::Red)
                            cont.setFillColor(sf::Color::Yellow);
                        else
                            cont.setFillColor(sf::Color::Red);
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
                    {
                        swoosh.stop();
                        swoosh.play();
                        window.clear();
                        displayInstruction(window);
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                    {
                        window.close();
                    }
                default:
                    ;
            }
        }
        window.clear(sf::Color(237,239,203));
        window.draw(background);
        window.draw(sent1);
        window.draw(sent2);
        window.draw(sent3);
        window.draw(sent4);
        window.draw(sent5);
        window.draw(cont);
        window.display();
    }
}

void displayInstruction (sf::RenderWindow& window)
{
    sf::Texture backgroundTexture;
    sf::Sprite background;
    
    backgroundTexture.loadFromFile(roadImage);
    background.setTexture(backgroundTexture);
    background.setScale(1.22,1.18);
    background.setTextureRect(sf::IntRect(0, 0, 1024, 768));
    background.setPosition(0.0f,0.0f);
    background.setColor(sf::Color(255, 255, 255, 198));
    
    sf::Font font;
    if (!font.loadFromFile(storyFont))
        std::cout << "Can't find font" << std::endl;
    
    sf::Font font2;
    if (!font2.loadFromFile(mainFont))
        std::cout << "Can't find font" << std::endl;
    
    sf::Text sent1;
    sent1.setFont(font);
    sent1.setString("Press A to go LEFT, and D to go RIGHT");
    sent1.setCharacterSize(37);
    sent1.setFillColor(sf::Color(232,146,79));
    sent1.setOutlineColor(sf::Color::Black);
    sent1.setOutlineThickness(7.0f);
    sent1.setStyle(sf::Text::Bold);
    sent1.setPosition(30.0f,80);
    
    sf::Text sent2;
    sent2.setFont(font);
    sent2.setString("Avoid these obstacles...You will lose a heart!");
    sent2.setCharacterSize(37);
    sent2.setFillColor(sf::Color(232,146,79));
    sent2.setOutlineColor(sf::Color::Black);
    sent2.setOutlineThickness(7.0f);
    sent2.setStyle(sf::Text::Bold);
    sent2.setPosition(30.0f,180);
    
    sf::Text sent3;
    sent3.setFont(font);
    sent3.setString("Hit a gift to recover a heart!");
    sent3.setCharacterSize(37);
    sent3.setFillColor(sf::Color(232,146,79));
    sent3.setOutlineColor(sf::Color::Black);
    sent3.setOutlineThickness(7.0f);
    sent3.setStyle(sf::Text::Bold);
    sent3.setPosition(30.0f,360);
    
    sf::Text sent5;
    sent5.setFont(font);
    sent5.setString("Your destination is 100,000 ft away...Good luck!");
    sent5.setCharacterSize(37);
    sent5.setFillColor(sf::Color(232,146,79));
    sent5.setOutlineColor(sf::Color::Black);
    sent5.setOutlineThickness(7.0f);
    sent5.setStyle(sf::Text::Bold);
    sent5.setPosition(30.0f,530);
    
    sf::Text cont;
    cont.setFont(font2);
    cont.setString("Back to menu...");
    cont.setCharacterSize(35);
    cont.setFillColor(sf::Color::Yellow);
    cont.setOutlineColor(sf::Color::Black);
    cont.setOutlineThickness(7.0f);
    cont.setStyle(sf::Text::Bold);
    cont.setOrigin(cont.getLocalBounds().left + cont.getLocalBounds().width, cont.getLocalBounds().top + cont.getLocalBounds().height);
    cont.setPosition(924.0f,720.0f);
    
    static sf::Texture frog;
    static sf::Texture box;
    static sf::Texture rock;
    static sf::Texture jayWalker;
    static sf::RectangleShape obstacles[4];
    
    frog.loadFromFile(frogImage,sf::IntRect(0, 3, 15, 13));
    box.loadFromFile(boxImage);
    rock.loadFromFile(rockImage);
    jayWalker.loadFromFile(walkerImage, sf::IntRect(91,19,44,67));
    
    obstacles[0].setTexture(&rock);
    obstacles[1].setTexture(&frog);
    obstacles[2].setTexture(&jayWalker);
    obstacles[3].setTexture(&box);
    
    obstacles[0].setSize (sf::Vector2f(80.0f,80.0f));
    obstacles[0].setPosition(272.0f, 255.0f);
    obstacles[1].setSize (sf::Vector2f(80.0f,80.0f));
    obstacles[1].setPosition(472.0f, 255.0f);
    obstacles[2].setSize (sf::Vector2f(80.0f,80.0f));
    obstacles[2].setPosition(672.0f, 255.0f);
    obstacles[3].setSize (sf::Vector2f(80.0f,80.0f));
    obstacles[3].setPosition(272.0f, 425.0f);
    
    static sf::Music swoosh;
    static bool loadMusic = true;
    if (loadMusic)
    {
        swoosh.openFromFile(swooshSound);
        loadMusic = false;
    }
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition( window );
            sf::Vector2f mousePosF( static_cast<float>( mousePos.x ), static_cast<float>( mousePos.y ) );
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseMoved:
                    if (cont.getGlobalBounds().contains( mousePosF ))
                        cont.setFillColor(sf::Color::Red);
                    else
                        cont.setFillColor(sf::Color::Yellow);
                case sf::Event::MouseButtonPressed:
                    if (cont.getGlobalBounds().contains( mousePosF ))
                        cont.setFillColor(sf::Color::Red);
                    else
                        cont.setFillColor(sf::Color::Yellow);
                    break;
                case sf::Event::MouseButtonReleased:
                    if (cont.getGlobalBounds().contains( mousePosF ))
                    {
                        swoosh.stop();
                        swoosh.play();
                        cont.setFillColor(sf::Color::Red);
                        window.clear();
                        displayMenu(window);
                    }
                case sf::Event::KeyPressed:
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                    {
                        if (cont.getFillColor()==sf::Color::Red)
                            cont.setFillColor(sf::Color::Yellow);
                        else
                            cont.setFillColor(sf::Color::Red);
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
                    {
                        swoosh.stop();
                        swoosh.play();
                        window.clear();
                        displayMenu(window);
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                    {
                        window.close();
                    }
                default:
                    ;
            
            }
        }
        window.clear(sf::Color(237,239,203));
        window.draw(background);
        window.draw(sent1);
        window.draw(sent2);
        window.draw(obstacles[0]);
        window.draw(obstacles[1]);
        window.draw(obstacles[2]);
        window.draw(sent3);
        window.draw(obstacles[3]);
        window.draw(sent5);
        window.draw(cont);
        window.display();
    }
}

void displayResult(sf::RenderWindow& window, int finalScore)
{
    sf::Texture backgroundTexture;
    sf::Sprite background;
    
    backgroundTexture.loadFromFile(roadImage);
    background.setTexture(backgroundTexture);
    background.setScale(1.22,1.18);
    background.setTextureRect(sf::IntRect(0, 0, 1024, 768));
    background.setPosition(0.0f,0.0f);
    background.setColor(sf::Color(255, 255, 255, 198));
    
    sf::Font font;
    if (!font.loadFromFile(mainFont))
        std::cout << "Can't find font" << std::endl;
    sf::Font font2;
    if (!font2.loadFromFile(storyFont))
        std::cout << "Can't find font" << std::endl;
    
    sf::Text title;
    title.setFont(font);
    title.setString("YOUR SCORE:");
    title.setCharacterSize(80);
    title.setFillColor(sf::Color(232,112,27));
    title.setOutlineColor(sf::Color::Black);
    title.setOutlineThickness(10.0f);
    title.setStyle(sf::Text::Bold);
    title.setOrigin(title.getLocalBounds().left + title.getLocalBounds().width / 2.0f, title.getLocalBounds().top + title.getLocalBounds().height / 2.0f);
    title.setPosition(1024/2.0f, 160);
    
    sf::Text fs;
    fs.setFont(font2);
    fs.setString(to_string(finalScore));
    fs.setCharacterSize(80);
    fs.setFillColor(sf::Color::White);
    fs.setOutlineColor(sf::Color::Black);
    fs.setOutlineThickness(10.0f);
    fs.setStyle(sf::Text::Bold);
    fs.setOrigin(fs.getLocalBounds().left + fs.getLocalBounds().width / 2.0f, fs.getLocalBounds().top + fs.getLocalBounds().height / 2.0f);
    fs.setPosition(1024/2.0f,280);
    
    Menu menu(window, "RETRY", "SCOREBOARD", "MENU");
    
    static sf::Music resultMusic;
    static sf::Music swoosh;
    static bool loadMusic = true;
    if (loadMusic)
    {
        swoosh.openFromFile(swooshSound);
        resultMusic.openFromFile(resultSound);
        resultMusic.setLoop(true);
        loadMusic = false;
    }
    
    resultMusic.stop();
    resultMusic.play();
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition( window );
            sf::Vector2f mousePosF( static_cast<float>( mousePos.x ), static_cast<float>( mousePos.y ) );
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseMoved:
                {
                    for (int i=0; i<3; i++)
                    {
                        if (menu.item[i].getGlobalBounds().contains( mousePosF ))
                        {
                            menu.item[i].setFillColor(sf::Color::Red);
                            if (i>0)
                                menu.setSelectedItem(i);
                        }
                        else
                            menu.item[i].setFillColor(sf::Color::Yellow);
                    }
                }
                    break;
                case sf::Event::MouseButtonPressed:
                {
                    for (int i=0; i<3; i++) {
                        if (menu.item[i].getGlobalBounds().contains( mousePosF ))
                        {
                            if (menu.item[i].getGlobalBounds().contains( mousePosF ))
                                menu.item[i].setFillColor(sf::Color::White);
                            else
                                menu.item[i].setFillColor(sf::Color::Yellow);
                        }
                    }
                }
                    break;
                case sf::Event::MouseButtonReleased:
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition( window );
                    sf::Vector2f mousePosF( static_cast<float>( mousePos.x ), static_cast<float>( mousePos.y ) );
                    if (menu.item[0].getGlobalBounds().contains( mousePosF ))
                    {
                        resultMusic.stop();
                        swoosh.stop();
                        swoosh.play();
                        menu.item[0].setFillColor(sf::Color::Red);
                        window.clear();
                        playGame(window);
                    }
                    if (menu.item[1].getGlobalBounds().contains( mousePosF ))
                    {
                        swoosh.stop();
                        swoosh.play();
                        menu.item[1].setFillColor(sf::Color::Red);
                        window.clear();
                        displayScoreboard(window);
                    }
                    if (menu.item[2].getGlobalBounds().contains( mousePosF ))
                    {
                        swoosh.stop();
                        swoosh.play();
                        resultMusic.stop();
                        menu.item[2].setFillColor(sf::Color::Red);
                        window.clear();
                        displayMenu(window);
                    }
                }
                    break;
                    
                case sf::Event::KeyPressed:
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                    {
                        menu.moveUp();
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                    {
                        menu.moveDown();
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                    {
                        window.close();
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
                    {
                        switch (menu.getSelectedItem())
                        {
                            case 0:
                                swoosh.stop();
                                swoosh.play();
                                resultMusic.stop();
                                window.clear();
                                playGame(window);
                            case 1:
                                swoosh.stop();
                                swoosh.play();
                                window.clear();
                                displayScoreboard(window);
                            case 2:
                                swoosh.stop();
                                swoosh.play();
                                resultMusic.stop();
                                window.clear();
                                displayMenu(window);
                            default:
                                ;
                        }
                    }
                default:;
            }

        }
        window.clear(sf::Color(237,239,203));
        window.draw(background);
        window.draw(title);
        window.draw(fs);
        menu.draw(window);
        window.display();
    }
}

void displayScoreboard(sf::RenderWindow& window)
{
    scoreboard.clear();
    readScoreboard();
    
    sf::Texture backgroundTexture;
    sf::Sprite background;
    
    backgroundTexture.loadFromFile(roadImage);
    background.setTexture(backgroundTexture);
    background.setScale(1.22,1.18);
    background.setTextureRect(sf::IntRect(0, 0, 1024, 768));
    background.setPosition(0.0f,0.0f);
    background.setColor(sf::Color(255, 255, 255, 198));
    
    sf::Font font;
    if (!font.loadFromFile(mainFont))
        std::cout << "Can't find font" << std::endl;
    sf::Font font2;
    if (!font2.loadFromFile(scoreboardFont))
        std::cout << "Can't find font" << std::endl;
    
    sf::Text title;
    title.setFont(font);
    title.setString("Scoreboard");
    title.setCharacterSize(70);
    title.setFillColor(sf::Color(232,112,27));
    title.setOutlineColor(sf::Color::Black);
    title.setOutlineThickness(7.0f);
    title.setStyle(sf::Text::Bold);
    title.setOrigin(title.getLocalBounds().left + title.getLocalBounds().width / 2.0f, title.getLocalBounds().top + title.getLocalBounds().height / 2.0f);
    title.setPosition(1024/2.0f,70);
    
    int scoreboardSize = scoreboard.size();
    if (scoreboardSize > 10) scoreboardSize = 10;
    sf::Text text[scoreboardSize];
    string nameStr, scoreStr, rankStr;
    string textStr;
    
    static sf::Music swoosh;
    static bool loadMusic = true;
    if (loadMusic)
    {
        swoosh.openFromFile(swooshSound);
        loadMusic = false;
    }
    
    auto it = scoreboard.begin();
    for (int i = 0; i < scoreboardSize; i++)
    {
        nameStr = (*it).getName(); scoreStr = to_string( (*it).getScore() );
        while (nameStr.size() < 16) nameStr += ".";
        while (scoreStr.size() < 6) scoreStr = "." + scoreStr;
        if (i == 9) rankStr = to_string(i+1);
        else rankStr = " " + to_string(i+1);
        
        textStr = rankStr + " " + nameStr + "..." + scoreStr;
        text[i].setFont(font2);
        text[i].setString(textStr);
        text[i].setCharacterSize(40);
        text[i].setFillColor(sf::Color(255, 209, 153));
        text[i].setOutlineColor(sf::Color::Black);
        text[i].setOutlineThickness(6.0f);
        text[i].setStyle(sf::Text::Bold);
        text[i].setOrigin(text[i].getLocalBounds().left + text[i].getLocalBounds().width / 2.0f, text[i].getLocalBounds().top + text[i].getLocalBounds().height / 2.0f);
        text[i].setPosition(1024/2.0f,155 + i*53);
        it++;
    }
    
    sf::Text cont;
    cont.setFont(font);
    cont.setString("Back to menu...");
    cont.setCharacterSize(35);
    cont.setFillColor(sf::Color::Yellow);
    cont.setOutlineColor(sf::Color::Black);
    cont.setOutlineThickness(7.0f);
    cont.setStyle(sf::Text::Bold);
    cont.setOrigin(cont.getLocalBounds().left + cont.getLocalBounds().width, cont.getLocalBounds().top + cont.getLocalBounds().height);
    cont.setPosition(924.0f,720.0f);
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition( window );
            sf::Vector2f mousePosF( static_cast<float>( mousePos.x ), static_cast<float>( mousePos.y ) );
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseMoved:
                    if (cont.getGlobalBounds().contains( mousePosF ))
                        cont.setFillColor(sf::Color::Red);
                    else
                        cont.setFillColor(sf::Color::Yellow);
                case sf::Event::MouseButtonPressed:
                    if (cont.getGlobalBounds().contains( mousePosF ))
                        cont.setFillColor(sf::Color::Red);
                    else
                        cont.setFillColor(sf::Color::Yellow);
                    break;
                case sf::Event::MouseButtonReleased:
                    if (cont.getGlobalBounds().contains( mousePosF ))
                    {
                        swoosh.stop();
                        swoosh.play();
                        cont.setFillColor(sf::Color::Red);
                        window.clear();
                        return;
                    }
                case sf::Event::KeyPressed:
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                    {
                        if (cont.getFillColor()==sf::Color::Red)
                            cont.setFillColor(sf::Color::Yellow);
                        else
                            cont.setFillColor(sf::Color::Red);
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
                    {
                        swoosh.stop();
                        swoosh.play();
                        window.clear();
                        return;
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                    {
                        window.close();
                    }
                default:
                    ;
                    
            }
        }
        window.clear(sf::Color(237,239,203));
        window.draw(background);
        window.draw(title);
        for (sf::Text tempText : text)
        {
            window.draw(tempText);
        }
        window.draw(cont);
        window.display();
    }
}

void moveObstacles(sf::RenderWindow& win, list<Obstacle*>& obstacleList, bool speedChanged, float deltaTime)
{
    for (int i = 0; i < obstacleList.size(); i++)
    {
        if (speedChanged) obstacleList.front()->setSpeed(obstacleList.front()->getSpeed() + 0.1f);
        if (obstacleList.front()->move(deltaTime))
        {
            Obstacle *temp = obstacleList.front();
            win.draw(*temp);
            obstacleList.push_back(obstacleList.front());
        }
        obstacleList.pop_front();
    }
}

void road(sf::RenderWindow& win, float speed)
{
    static sf::Texture roadtexture;
    static sf::RectangleShape road[2];
    static sf::Clock ck;
    static sf::Time timer = sf::Time::Zero;
    static bool makeroad = true;
    if(makeroad)
    {
        roadtexture.loadFromFile(roadImage);
        road[0].setSize(sf::Vector2f(1024.0f,768.0f));
        road[1].setSize(sf::Vector2f(1024.0f,768.0f));
        road[0].setPosition(0.0f,-768.0f);
        road[1].setPosition(0.0f,0.0f);
        road[0].setTexture(&roadtexture);
        road[1].setTexture(&roadtexture);
        makeroad = false;
    }
    
    timer = ck.restart();
    if(timer.asSeconds() >= 10.0f)
        makeroad = true;
    if(timer.asSeconds() >= 0.5f)
        timer = sf::Time::Zero;
    if(road[0].getPosition().y >= 768.0f)
        road[0].setPosition(0.0f,-768.0f);
    road[0].move(0.0f,speed * timer.asMilliseconds());
    if(road[1].getPosition().y >= 768.0f)
        road[1].setPosition(0.0f,-768.0f);
    road[1].move(0.0f,speed * timer.asMilliseconds());
    win.draw(road[0]);
    win.draw(road[1]);
}

int calculateScore(sf::RenderWindow& win, int level, int timeInHundredth, int currentScore)
{
    int scoreInt;
    if (timeInHundredth >= 5750 || currentScore == 100000) scoreInt = 100000;
    else scoreInt = currentScore + timeInHundredth*level/2;

    return scoreInt;
}

void showScore(sf::RenderWindow& win, int level, int finalScore)
{
    sf::Font font;
    if (!font.loadFromFile(scoreFont))
        std::cout << "Can't find font" << std::endl;
    
    int scoreIntCopy = finalScore;
    int countDigits = 0;
    while (scoreIntCopy!=0) { countDigits++; scoreIntCopy /= 10; }
    string scoreStr = "";
    for (int i = 0; i<(6-countDigits); i++) {
        scoreStr += "0";
    }
    
    scoreStr += to_string(finalScore);
    
    sf::Text score;
    score.setFont(font);
    score.setString(scoreStr);
    score.setCharacterSize(120);
    score.setFillColor(sf::Color::Black);
    score.setOrigin(score.getLocalBounds().width,0.0f);
    score.setPosition(1020.0f,-60.0f);
    
    sf::Text stage;
    stage.setFont(font);
    stage.setString("STAGE " + to_string(level));
    stage.setCharacterSize(100);
    stage.setFillColor(sf::Color::Black);
    stage.setStyle(sf::Text::Bold);
    stage.setOrigin(0.0f,stage.getLocalBounds().height);
    stage.setPosition(4.0f,700.0f);
    
    win.draw(score);
    win.draw(stage);
    
}

void lives(sf::RenderWindow& win, int lives)
{
    static sf::Texture heartTexture;
    static sf::RectangleShape heart[5];
    static bool makehearts = true;
    
    if(makehearts)
    {
        heartTexture.loadFromFile(heartImage);
        heart[0].setSize(sf::Vector2f(80.0f,80.0f));
        heart[1].setSize(sf::Vector2f(80.0f,80.0f));
        heart[2].setSize(sf::Vector2f(80.0f,80.0f));
        heart[3].setSize(sf::Vector2f(80.0f,80.0f));
        heart[4].setSize(sf::Vector2f(80.0f,80.0f));
        heart[0].setPosition(0.0f,2.0f);
        heart[1].setPosition(80.0f,2.0f);
        heart[2].setPosition(160.0f,2.0f);
        heart[3].setPosition(240.0f,2.0f);
        heart[4].setPosition(320.0f,2.0f);
        makehearts = false;
    }
    
    for (auto i = 0; i<lives; i++)
    {
        heart[i].setTexture(&heartTexture);
        win.draw(heart[i]);
    }
}
