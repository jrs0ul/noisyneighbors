#ifndef BRAT_H
#define BRAT_H

#include "Vectors.h"
#include "TextureLoader.h"
#include "ProjectileList.h"
#include "OldMan.h"
#include "Door.h"
#include "audio/SoundSystem.h"

struct Brat{
    Vector3D pos;
    bool dead;
    unsigned frame;
    int animtics;
    bool runleft;
    int blatics;
    int hp;
    bool killed;
    bool damaged;
    int damagetics;
   
    void reset(){
        pos = Vector3D(160, 55, 0);
        dead = false;
        frame = 0;
        animtics = 0;
        runleft = false;
        blatics = 0;
        hp =  3;
        killed = false;
        damagetics = 0;
        damaged = false;

    }

    Brat(){
        reset();
    };
    
    void draw(PicsContainer& pics, unsigned spriteIndex);
    void AI(ProjectileList& pr, OldMan& man, Door& door, SoundSystem& ss);
};





#endif //BRAT_H
