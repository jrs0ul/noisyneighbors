#ifndef PROJECTILE_LIST_H
#define PROJECTILE_LIST_H

#include "DArray.h"
#include "Projectile.h"

class ProjectileList{

    DArray<Projectile> projectiles;
public:
    void add(Projectile p){projectiles.add(p);}
    void remove(unsigned index){projectiles.remove(index);}
    Projectile * get(unsigned index){if (index < projectiles.count()) return &projectiles[index]; return 0;}
    void update(Vector3D target, int& hp, bool& damaged, int& dmgtics );
    void draw(PicsContainer& pics);
    void destroy(){projectiles.destroy();}
    unsigned long count(){return projectiles.count();}

};




#endif //PROJECTILE_LIST_H
