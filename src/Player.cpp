#include "Player.h"



void Player::drawScore(PicsContainer& pics, unsigned font, float x, float y){
    char buf[100];
    sprintf(buf, "%ld", _score);
    WriteText(x, y, pics, font, buf);
    
}

//-----------------------------
void Player::drawLives(PicsContainer& pics, unsigned shape, float x, float y){
    
    for (int i = 0; i < _lives; i++){
        pics.draw(shape, x + 16 * i, y, 0, false, 0.5f, 0.5f); 
    }
}
