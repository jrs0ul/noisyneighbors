#ifndef DRILLER_H
#define DRILLER_H

#include "Vectors.h"
#include "TextureLoader.h"
#include "ProjectileList.h"
#include "OldMan.h"
#include "audio/SoundSystem.h"

struct Driller{

    Vector3D pos;
    unsigned frame;
    bool dead;
    bool drilling;
    bool resting;
    int animtics;
    int drillingtics;
    int restingtics;
    int drrtics;
    bool killed;
    int hp;

    unsigned buffer;


    void reset(){
        if (alIsSource(buffer)){
            alSourceStop(buffer);
            //alDeleteSources(1, &buffer);
        }
        frame = 1;
        dead = false;
        drilling = false;
        resting = false;
        animtics = 0;
        drillingtics = 0;
        restingtics = 0;
        drrtics = 0;
        killed = false;
        hp = 3;

    }
    Driller(){
        reset();
    }

    void draw(PicsContainer& pics, unsigned spriteIndex);
    void AI(ProjectileList& pr, OldMan& man, SoundSystem& ss);

};





#endif //DRILLER_H
