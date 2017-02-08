#include "OldMan.h"
#include "externals.h"

void OldMan::draw(PicsContainer& pics,
                  unsigned manSprite, unsigned handSprite){

    pics.draw(handSprite, pos.v[0] + handpos.v[0], pos.v[1] + handpos.v[1],
              0, true, handScaleX, 0.5f, handAngle,
              (damaged)?COLOR(1,0,0):COLOR(1,1,1),
              (damaged)?COLOR(1,0,0):COLOR(1,1,1));
    pics.draw(manSprite, pos.v[0], pos.v[1], frame, false, 1,1,0,
              (damaged)?COLOR(1,0,0):COLOR(1,1,1),
              (damaged)?COLOR(1,0,0):COLOR(1,1,1));
}
//---------------------------------
void OldMan::damageAnim(){
    if (damaged){
        damagetics++;
        if (damagetics > 5){
            damagetics = 0;
            damaged = false;
        }
    }

}
//---------------------------------
void OldMan::animate(unsigned first, unsigned last){
    frametic++;
    if (frametic > 5){
        frame++;
        frametic = 0;
        if (frame > last-1)
            frame = first;
    }

   }
//-------------------------------
void OldMan::moveLeft(){
     if (pos.v[0] > MINROOMX)
        pos.v[0]-= 2;
        
    if (frame > 5)
        frame = 0;
    animate(0, 4);
    handScaleX = 0.6f;
    handAngle = -100;
    handpos = Vector3D(20, 40, 0);

}
//------------------------------
void OldMan::moveRight(){
    if (pos.v[0] + 64 < MAXROOMX)
        pos.v[0]+= 2;
    if (frame < 5)
        frame = 5;
    animate(5, 9);
    handScaleX = -0.6f;
    handAngle = 100;
    handpos = Vector3D(45, 40, 0);
}
//------------------------------
void OldMan::attack(ProjectileList& pl, SoundSystem& ss){
    attacktics++;
    handAngle = 0;
    if ((frame == 4)||(frame == 9)){
        handpos = (frame > 4) ? Vector3D(55, 10, 0) : Vector3D(10, 10, 0);
    }
    else{
        handpos = (frame > 4) ? Vector3D(55, 30, 0) : Vector3D(10,30,0);
    }
    if (attacktics > 35){
        attacktics = 0;
        shot = false;
        attacks = false;
        handAngle = (frame > 4) ? 100 : -100;
        handpos = (frame > 4) ? Vector3D(45, 40, 0) : Vector3D(20, 40, 0);
    }

    if ((attacktics > 10)&&(attacktics < 20)&&(!shot)){

        Projectile p;

        bool hitswall = false;
        if (frame > 4){
            if ((pos.v[0] + 70 > MAXROOMX)&&(pos.v[0]+70 < MAXROOMX + 20)){
                hitswall = true;
                p.pos = pos + Vector3D(70, 15, 0);
                p.direction = Vector3D(1, 0 , 0);
                p.angle = 180;
            }

            if ((pos.v[1] - 20 < MINROOMY)&&(pos.v[1]-20 > MINROOMY - 20)){
                hitswall = true;
                p.pos = pos + Vector3D(70, -5, 0);
                p.direction = Vector3D(0, -1 , 0);
                p.angle = 90;
            }
        }
        else{
            if ((pos.v[0] - 10 < MINROOMX) && (pos.v[0] - 10 > MINROOMX - 20)){
                hitswall = true;
                p.pos = pos + Vector3D(-10, 15, 0);
                p.direction = Vector3D(-1, 0 , 0);
                p.angle = 0;
            }

            if ((pos.v[1] - 20 < MINROOMY)&&(pos.v[1]-20 > MINROOMY - 20)){
                hitswall = true;
                p.pos = pos + Vector3D(-10, -5, 0);
                p.direction = Vector3D(0, -1 , 0);
                p.angle = 90;
            }
        }

        if (hitswall){
            p.picIndex = 11;
            p.speed = 2.8f;
            pl.add(p);
            ss.playsound(3);
            shot = true;
        }
    }
}
