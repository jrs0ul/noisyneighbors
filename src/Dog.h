#ifndef DAWG_H
#define DAWG_H

#include "TextureLoader.h"
#include "Vectors.h"
#include "ProjectileList.h"
#include "OldMan.h"
#include "audio/SoundSystem.h"
#include "Door.h"

struct Dog{

    Vector3D pos;
    bool dead;
    int animtics;
    unsigned frame;
    bool runleft;
    bool run;
    bool sit;
    int runtics;
    int sittics;
    int barktics;
    bool killed;
    int hp;
    int damagetics;
    bool damaged;


    void reset(){
        pos = Vector3D(150, 72, 0);
        dead = false;
        animtics = 0;
        frame = 1;
        runleft = false;
        runtics = 0;
        sittics = 0;
        run = true;
        sit = false;
        barktics = 0;
        hp = 3;
        killed = false;
        damagetics = 0;
        damaged = false;
    }

    Dog(){reset();}

    void draw(PicsContainer& pics, unsigned spriteIndex);
    void AI(ProjectileList& pr, OldMan& man, Door& door, SoundSystem& ss);
};



#endif //DAWG_H
