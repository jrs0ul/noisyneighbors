#include "Brat.h"
#include "externals.h"


void Brat::draw(PicsContainer& pics, unsigned spriteIndex){
    pics.draw(spriteIndex, pos.v[0], pos.v[1], frame, false, 1, 1, 0,
              (damaged)?COLOR(1,0,0):COLOR(1,1,1),
              (damaged)?COLOR(1,0,0):COLOR(1,1,1));
}
//-------------------------
void Brat::AI(ProjectileList& pr, OldMan& man, Door& door, SoundSystem& ss){
    if (!dead){

        if (damaged){
            damagetics++;
            if (damagetics > 4){
                damagetics = 0;
                damaged = false;
            }

        }

        animtics++;
        if (!runleft){
            pos.v[0] += 1;
            if (pos.v[0] + 64 >= MAXROOMX )
                runleft = true;
        }
        else{
            pos.v[0] -= 1;
            if (pos.v[0] <= MINROOMX )
                runleft = false;
        }

        if (animtics > 10){
            frame++;
            animtics = 0;
            if (!runleft){
                if (frame > 3)
                    frame = 0;
            }
            else{
                if (frame > 7)
                    frame = 4;
            }
        }

        if (!killed){
            blatics++;
            if (blatics > 90){
                blatics = 0;
                Projectile p;
                p.pos = pos + Vector3D(32,10,0);
                p.picIndex = 7;
                p.direction = (man.pos+Vector3D(32,40,0)) - p.pos;
                p.direction.normalize();
                p.speed = 1.0f;
                p.radius = 8;
                p.value = 3;
                pr.add(p);
                ss.playsound(1);
            }

        }

        if (killed){
            if (pos.v[0] + 32 > 170)
                pos.v[0] -= 2;
            if (pos.v[0] + 32 < 170)
                pos.v[0] += 2;
            if ((pos.v[0] + 32 > 160)&&(pos.v[0]+32 < 180)){
                if ((!door.closeIt)&&(!door.openIt)&&(!door.open)){
                    door.openIt = true;
                    //puts("bbb");
                }
                if (door.open){
                    //puts("aaa");
                    dead = true;
                    //door.closeIt = true;
                }
            }
        }
    }
}
