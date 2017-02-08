#include "Door.h"


void Door::draw(PicsContainer& pics, unsigned spriteIndex){
    pics.draw(spriteIndex, pos.v[0], pos.v[1], frame, false);
}
//--------------------------------------
void Door::AI(ProjectileList& pl, OldMan& man, SoundSystem& ss){

    if ((openIt)&&(!open)){
        animtics++;
        if (animtics > 20){
            animtics = 0;
            if (frame == 0){
                frame = 1;
            }
            else{
                frame = 2;
                open = true;
                openIt = false;
            }
        }
    }

    if ((frame == 2)||(closeIt)){

        animtics++;
        if (animtics > 20){
            animtics = 0;
            if (frame == 2){
                open = false;
                closeIt = true;
                frame = 1;
            }
            else {
                frame = 0;
                closeIt = false;
                Projectile p;
                p.pos = pos + Vector3D(50,50,0);
                p.picIndex = 12;
                p.direction = (man.pos+Vector3D(32,40,0)) - p.pos;
                p.direction.normalize();
                p.speed = 1.5f;
                p.radius = 20;
                p.value = 10;
                pl.add(p);
                ss.playsound(4);
            }
        }
    }

}
