#include "Projectile.h"
#include "Utils.h"

void Projectile::draw(PicsContainer& pics){
    pics.draw(picIndex, pos.v[0], pos.v[1], 0, true, 0.5f, 0.5f, angle);
}
//-----------------------------
void Projectile::update(Vector3D target, 
                        int& hp, bool& damaged, int& dmgtics){
    pos = pos + Vector3D(direction.v[0]*speed, direction.v[1]*speed, 0);


    if (picIndex != 11){
        if (CirclesColide(target.v[0] + 32, target.v[1] + 40, 32, pos.v[0], pos.v[1], radius)){
            dead = true;
            damaged = true;
            dmgtics = 0;
            hp -= value;
        }
    }

    if (!dead){

         if (pos.v[1] > 295)
            dead = true;

        if (pos.v[0] < -30)
            dead = true;

        if (pos.v[0] > 480)
            dead = true;
    }
}

