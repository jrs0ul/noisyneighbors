#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Vectors.h"
#include "TextureLoader.h"

struct Projectile{
    Vector3D pos;
    Vector3D direction;
    float speed;
    float angle;
    bool dead;
    unsigned picIndex;
    int value;
    float radius;

    Projectile(){
        picIndex = 6;
        dead = false;
        angle = 0;
        value = 2;
        radius = 8;
    }

    void draw(PicsContainer& pics);
    void update(Vector3D target, int& hp, bool& damaged, int& dmgtics);
};



#endif //PROJECTILE_H
