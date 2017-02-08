#include "Dog.h"
#include "externals.h"

void Dog::draw(PicsContainer& pics, unsigned spriteIndex){
    pics.draw(spriteIndex, pos.v[0], pos.v[1], frame, false, 1, 1, 0,
              (damaged)?COLOR(1,0,0):COLOR(1,1,1),
              (damaged)?COLOR(1,0,0):COLOR(1,1,1));
}
//-------------------------
void Dog::AI(ProjectileList& pr, OldMan& man, Door& door, SoundSystem& ss){
    if (!dead){

        if (damaged){
            damagetics++;
            if (damagetics > 4){
                damagetics = 0;
                damaged = false;
            }

        }
        

        if (run){
            animtics++;
            if (!runleft){
                pos.v[0] += 2;
                if (pos.v[0] + 64 >= MAXROOMX )
                    runleft = true;
            }
            else{
                pos.v[0] -= 2;
                if (pos.v[0] <= MINROOMX )
                    runleft = false;
            }

            if (animtics > 8){
                frame++;
                animtics = 0;
                if (!runleft){
                    if (frame > 4)
                        frame = 1;
                }
                else{
                    if (frame > 9)
                        frame = 6;
                }
            }

            runtics++;
            if (runtics > 270){
                run = false;
                sit = true;
                runtics = 0;
            }
        } //run

        if (sit){
            sittics++;
            frame = (frame > 4) ? 5 : 0;

            barktics++;
            if (barktics > 30){
                barktics = 0;
                Projectile p;
                p.pos = pos + Vector3D(10,10,0);
                p.direction = (man.pos+Vector3D(32,40,0)) - p.pos;
                p.direction.normalize();
                p.speed = 1.5f;
                p.radius = 12;
                pr.add(p);
                ss.playsound(0);
            }

            if (sittics > 110){
                sit = false;
                run = true;
                sittics = 0;
                animtics = 0;
            }
        }

        //--
        if (killed){
            animtics++;


            if (pos.v[0] + 22 > 170){
                pos.v[0] -= 2;
                runleft = true;
            }
            if (pos.v[0] + 22 < 170){
                pos.v[0] += 2;
                runleft = false;
            }


            if (animtics > 8){
                frame++;
                animtics = 0;
                if (!runleft){
                    if (frame > 4)
                        frame = 1;
                }
                else{
                    if (frame > 9)
                        frame = 6;
                }
            }


            if ((pos.v[0] + 22 > 160)&&(pos.v[0]+22 < 180)){
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
