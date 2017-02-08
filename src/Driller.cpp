#include "Driller.h"
#include "externals.h"

void Driller::draw(PicsContainer& pics, unsigned spriteIndex){
    pics.draw(spriteIndex, pos.v[0], pos.v[1], frame, false);
}
//-----------------------
void Driller::AI(ProjectileList& pr, OldMan& man, SoundSystem& ss){
    if (!dead){
        if ((!drilling)&&(!resting)&&(!killed)){
            animtics++;
            if (animtics > 10){
                frame++;
                animtics = 0;
                if (frame > 4+((pos.v[0] > MAXROOMX) ? 5 : 0))
                    frame = 1+((pos.v[0] > MAXROOMX) ? 5 : 0);
            }

            if (pos.v[0] > MAXROOMX + 10)
                pos.v[0]-= 0.1f;
            if ((pos.v[0] < MAXROOMX + 15)&&(pos.v[0] > MAXROOMX )){
                drilling = true;
                alSourcef(buffer, AL_GAIN, ss.getVolume());
                alSourcePlay(buffer);
                frame = 5;
            }

            if (pos.v[0] < MINROOMX - 85)
                pos.v[0]+= 0.1f;

            if ((pos.v[0] > MINROOMX - 88)&&(pos.v[0] < MINROOMX)){
                drilling = true;
                alSourcef(buffer, AL_GAIN, ss.getVolume());
                alSourcePlay(buffer);
                frame = 0;
            }

        }

        if (drilling){
            drillingtics++;


            drrtics++;
            if (drrtics > 100){
                drrtics = 0;
                Projectile p;
                p.pos = pos + Vector3D(((pos.v[0] > MAXROOMX)? 0 : 75), 25, 0);
                p.picIndex = 8;
                p.direction = (man.pos+Vector3D(32,40,0)) - p.pos;
                p.direction.normalize();
                p.speed = 1.0f;
                pr.add(p);

            }

            if (drillingtics > 410){
                drillingtics = 0;
                drilling = false;
                resting = true;
                frame++;
            }
        }

        if (resting){
            restingtics++;
            if (restingtics > 300){
                restingtics = 0;
                resting = false;
                drilling = true;
                alSourcef(buffer, AL_GAIN, ss.getVolume());
                alSourcePlay(buffer);
                frame--;
            }
        }

        if (killed){


            if ((pos.v[0] > -30)&&(pos.v[0] < MINROOMX))
                pos.v[0]-= 0.2f;
            if ((pos.v[0] < 470)&&(pos.v[0] > MAXROOMX))
                pos.v[0]+= 0.2f;
            if ((pos.v[0] < -30)||(pos.v[0] > 470))
                dead = true;
            animtics++;
            if (animtics > 10){
                frame++;
                animtics = 0;
                if (frame > 4+((pos.v[0] > MAXROOMX) ? 0 : 5))
                    frame = 1+((pos.v[0] > MAXROOMX) ? 0 : 5);
            }

            /*if (pos.v[0] > MAXROOMX + 10)
                pos.v[0]-= 0.1f;
            if ((pos.v[0] < MAXROOMX + 15)&&(pos.v[0] > MAXROOMX )){
                drilling = true;
                alSourcef(buffer, AL_GAIN, ss.getVolume());
                alSourcePlay(buffer);
                frame = 5;
            }

            if (pos.v[0] < MINROOMX - 85)
                pos.v[0]+= 0.1f;

            if ((pos.v[0] > MINROOMX - 88)&&(pos.v[0] < MINROOMX)){
                drilling = true;
                alSourcef(buffer, AL_GAIN, ss.getVolume());
                alSourcePlay(buffer);
                frame = 0;
            }

            dead = true;*/
        }

    }
}
