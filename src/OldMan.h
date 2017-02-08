#ifndef OLDMAN_H
#define OLDMAN_H

#include "Vectors.h"
#include "TextureLoader.h"
#include "ProjectileList.h"
#include "audio/SoundSystem.h"
struct OldMan{

    Vector3D pos;
    Vector3D handpos;
    float radius;
    unsigned frame;
    int frametic;
    bool dead;
    bool jump;
    int jumptics;
    float handAngle;
    float handScaleX;
    int attacktics;
    bool attacks;
    bool shot;
    bool damaged;
    int damagetics;

    int hp;


    void reset(){
        dead = false;
        frame = 0;
        frametic = 0;
        jump = false;
        jumptics = 0;
        hp = 100;
        handAngle = -100;// * 0.0174532925f;
        handScaleX = 0.6f;
        handpos = Vector3D(20, 40, 0);
        attacktics = 0;
        attacks = false;
        shot = false;
        damaged = false;
        damagetics = 0;
    }

    OldMan(){reset();}

    void draw(PicsContainer& pics, unsigned manSprite, unsigned handSprite);
    void animate(unsigned first, unsigned last);
    void moveLeft();
    void moveRight();
    void attack(ProjectileList& pl, SoundSystem& ss);
    void damageAnim();
};





#endif //OLDMAN_H
