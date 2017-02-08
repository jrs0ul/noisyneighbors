#ifndef DOOR_H
#define DOOR_H

#include "Vectors.h"
#include "TextureLoader.h"
#include "ProjectileList.h"
#include "OldMan.h"

struct Door{
    Vector3D pos;
    bool open;

    bool openIt;
    bool closeIt;

    unsigned frame;
    int animtics;

    Door(){
        animtics = 0;
        open = false;
        frame = 0;
        openIt = false;
        closeIt = false;
    }

    void draw(PicsContainer& pics, unsigned spriteIndex);
    void AI(ProjectileList& pl, OldMan& man, SoundSystem& ss);
};


#endif //DOOR_H
