#ifndef PLAYER_H
#define PLAYER_H

#include "TextureLoader.h"
#include "gui/Text.h"

class Player{
    long _score;
    int  _lives;
    unsigned currentLevel;
    
public:
    
    Player(){
        _score = 0;
        _lives = 3;
        currentLevel = 0;
    }
    
    void drawScore(PicsContainer& pics, unsigned font, float x, float y);
    void drawLives(PicsContainer& pics, unsigned shape, float x, float y);
    void resetScore(){_score = 0;}
    void resetLevel(){currentLevel = 0;}
    void setLevel(unsigned lev){currentLevel = lev;}
    void incLevel(){currentLevel++;}
    unsigned level(){return currentLevel;}
    long score(){return _score;}
    void addScore(long ns){_score += ns;}
    void resetLives(){_lives = 3;}
    void die(){if (_lives) _lives--;}
    int  lives(){return _lives;}
    
};

#endif //PLAYER_H

