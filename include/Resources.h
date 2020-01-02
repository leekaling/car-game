//
//  Resources.h
//  carGame
//
//  Created by Ethan Huang on 3/19/19.
//  Copyright © 2019 Ethan Huang. All rights reserved.
//

#ifndef Resources_h
#define Resources_h

#include <string>

using namespace std;

const string gameName = "Dangerous Driver";

const string filePrefix = "resources/";

namespace ImageFiles
{
    const string rockImage = filePrefix + "rock1.png";
    const string boxImage= filePrefix + "box1.png";
    const string explosionImage = filePrefix + "explosion2.png";
    const string frogImage = filePrefix + "frog2.png";
    const string walkerImage = filePrefix + "walker3.png";
    const string deadBodyImage = filePrefix + "deadbody.png";
    const string roadImage = filePrefix + "road.png";
    const string heartImage = filePrefix + "heart.png";
}

namespace SoundFiles
{
    const string explosionSound = filePrefix + "explosion1.wav";
    const string boxExplosionSound = filePrefix + "boxExplosion.wav";
    const string walkerExplosionSound = filePrefix + "walkerExplosion.wav";
    
    const string mainSound = filePrefix + "Background Music.ogg";
    const string backgroundSound = filePrefix + "Game Background Music.ogg";
    const string gameoverSound = filePrefix + "gameover.ogg";
    const string winSound = filePrefix + "win.ogg";
    const string swooshSound = filePrefix + "swoosh.ogg";
    const string resultSound = filePrefix + "end.ogg";
}

namespace FontFiles
{
    const string mainFont = filePrefix + "Sugar Bomb.ttf";
    const string nameFont = filePrefix + "Love Nature.ttf";
    const string storyFont = filePrefix + "GROBOLD.ttf";
    const string endFont = filePrefix + "waltographUI.ttf";
    const string scoreboardFont = filePrefix + "Hack-Bold.ttf";
    const string scoreFont = filePrefix + "novamono.ttf";
}

namespace TextFiles
{
    const string scoreboardText = filePrefix + "scoreboard.txt";
}









#endif /* Resources_h */
