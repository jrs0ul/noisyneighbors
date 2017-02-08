#include "ProjectileList.h"


void ProjectileList::update(Vector3D target, int& hp, bool& damaged, int& dmgtics){

    for (unsigned i = 0; i < projectiles.count(); i++){
        if (!projectiles[i].dead)
            projectiles[i].update(target, hp, damaged, dmgtics);
    }
    if (projectiles.count()){
        for (unsigned i = projectiles.count()-1; i > 0; i--){
            if (projectiles[i].dead)
                projectiles.remove(i);
        }
    }

}
//----------------------------
void ProjectileList::draw(PicsContainer& pics){
    for (unsigned i = 0; i < projectiles.count(); i++){
        if (!projectiles[i].dead)
            projectiles[i].draw(pics);
    }
}
