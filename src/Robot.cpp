
#include "Robot.h"
#include "Utils.h"
#include <cmath>

void Robot::spawnParticles(Vector3D hitterdir, float strength){
    particles.destroy();
    particles.setSystemLifetime((int)(10.0f * (strength * 0.8f)));
    particles.setParticleLifetime((int)(5.0f * (strength * 0.9f)));
    
    particles.setPos(pos.v[0] + radius * hitterdir.v[0], 0, pos.v[1] + radius * hitterdir.v[1]);
    particles.setDirIntervals(Vector3D(hitterdir.v[0], 0, hitterdir.v[1]), 180);
    particles.setColors(COLOR(1,1,0,1), COLOR(1,1,1,0.4));
    particles.setSizes(0.1f, 0.5f);
    particles.revive();
}
//---------------------------------
void Robot::regainConsciousness(){
    if (hit){
        hittic++;
        if (hittic > maxKnockDownTime){
            hit = false;
            hittic = 0;
        }
    }
}
//---------------------------------
void Robot::jump(){
    Vector3D v(direction.v[0] * speed,
               direction.v[1] * speed,
               direction.v[2] * speed);
    Vector3D j(0, 0, 1.8f);
    
    direction = v + j;
    speed = direction.length();
    direction.normalize();
    
}
//---------------------------------
void Robot::setSpriteDirection(Vector3D dir){
    spriteDirection = dir;
    spriteAngle = (3.14f - atan2(spriteDirection.v[0], spriteDirection.v[1]))*57.2957795f;
}

//---------------------------------
void Robot::draw(PicsContainer& pics, int x, int y, float _c,
                 int screenWidth, int screenHeight){
    
    float _x = x + pos.v[0];
    float _y = y + pos.v[1];
    
    if ((_x > -32) && (_x - radius < screenWidth) 
            && (_y > -32) && (_y - radius < screenHeight))
    
        pics.draw(face,
                  _x,
                  _y,
                  frame, true,
                  (radius + pos.v[2]/4.0f) / 16.0f,
                  (radius + pos.v[2]/4.0f) / 16.0f,
                  spriteAngle,
                  COLOR(c.c[0] * _c, c.c[1] * _c, c.c[2] * _c, c.c[3]),
                  COLOR(c.c[0] * _c, c.c[1] * _c, c.c[2] * _c, c.c[3]));
    
}
//--------------------------
bool Robot::shouldFall(LevelMap& map){
    
    int p[4][2];
    
    float rad = radius - (radius/4.0f);
    p[0][0] = (int)(pos.v[0] - rad)/32;
    p[0][1] = pos.v[1] / 32;
    p[1][0] = (int)(pos.v[0] + rad)/32;
    p[1][1] = p[0][1];
    
    p[2][0] = pos.v[0] / 32;
    p[2][1] = (int)(pos.v[1] - rad)/32;
    p[3][0] = p[2][0];
    p[3][1] = (int)(pos.v[1] + rad)/32;
    
    
    bool b[4];
    
    for (unsigned i = 0; i < 4; i++){
       
        if (((p[i][0] >= 0)&&(p[i][0] < (int)map.width))&&((p[i][1] >= 0)&&(p[i][1] < (int)map.height))){
            b[i] = (map.map[(unsigned)p[i][1]][(unsigned)p[i][0]].picture == 0) ? true : false;
          
        }
        else {
            b[i] = true;
            
        }
    }
   
    return (b[0] && b[1] && b[2] && b[3]) ? true : false;
}
//-------------------------
void Robot::ai_move(int maxSpeedGain, int minSpeedGain, float maxSpeed){
    Vector3D newdir = nextSquare - pos;
    newdir.normalize();
    spriteDirection = newdir;
    spriteAngle = (3.14f - atan2(spriteDirection.v[0], spriteDirection.v[1]))*57.2957795f;
    float newspeed = (rand() % maxSpeedGain + minSpeedGain)/100.0f ;
    if (!hit){
        direction = Vector3D(direction.v[0] * speed, direction.v[1] * speed, 0) 
        + Vector3D(newdir.v[0] * newspeed, newdir.v[1] * newspeed, 0);
        speed = (direction.length() > maxSpeed) ? maxSpeed : direction.length() ;
        direction.normalize();
    }
}
//--------------------------
void Robot::nextRandomSquare(LevelMap& map){
    Vector3D possibleValues[8];
    int valueCount = 0;
    
    int x = nextSquare.v[0]/32;
    int y = nextSquare.v[1]/32;
    
    for (int i = -1; i < 2; i++){
        for(int a = -1; a < 2; a++){
            if ((x + a > -1) && (x+a < (int)map.width) && ( a != 0 && i != 0) 
                && (y+i > -1) && (y+i < (int)map.height)){
                if (map.map[y+i][x+a].picture > 0){
                    possibleValues[valueCount] = Vector3D((x+a) * 32.0f + 16.0f, (y+i) * 32.0f + 16.0f, 0);
                    valueCount++;
                }
            }
        }
    }
    if (valueCount)
        nextSquare = possibleValues[rand() % valueCount];
}
//--------------------------
void Robot::makePath(LevelMap& map, const Robot * robots, unsigned long robotCount){
    _Point src;
    src.x = pos.v[0] / 32;
    src.y = pos.v[1] / 32;
    _Point dst;
    dst.x = (int)robots[0].pos.v[0] / 32;
    dst.y = (int)robots[0].pos.v[1] / 32;
    path.destroy();
    
    bool ** bmap = 0;
    map.boolean(&bmap);
    
    for (unsigned i = 0; i < robotCount; i++ ){
        if ((robots[i].mass >= mass)){
            int x = (robots[i].pos.v[0] + robots[i].direction.v[0] * (robots[i].radius - 2))/32;
            int y = (robots[i].pos.v[1] + robots[i].direction.v[1] * (robots[i].radius - 2))/32;
            if((x != src.x)&&(y != src.y)&&
               ((unsigned)y < map.height)&&((unsigned)x < map.width))
                bmap[y][x] = false;
            x = (robots[i].pos.v[0] - robots[i].direction.v[0] * (robots[i].radius - 2))/32;
            y = (robots[i].pos.v[1] - robots[i].direction.v[1] * (robots[i].radius - 2))/32;
            if((x != src.x)&&(y != src.y)&&
               ((unsigned)y < map.height)&&((unsigned)x < map.width))
                bmap[y][x] = false;
        }
    }
        
    
    path.findPath(src, dst, bmap, map.width, map.height);
    
    for (unsigned i = 0; i < map.height; i++)
        free(bmap[i]);
    free(bmap);
    pathIndex = 0;
}

//--------------------------
void Robot::ai(LevelMap& map, const Robot* robots, unsigned long robotCount){
    
    switch(face){
        case 5:{
            if (CirclesColide(pos.v[0], pos.v[1], radius/2.0f, nextSquare.v[0], nextSquare.v[1], 4.0f))
                nextRandomSquare(map);
            ai_move(42, 10, 2.8f);
            
        } break;
        //##########################    
        case 6:{
            
            if (CirclesColide(pos.v[0], pos.v[1], radius/2.0f, nextSquare.v[0], nextSquare.v[1], 3.0f)){
                
                if ((unsigned)pathIndex < path.parent.count())
                    pathIndex++;
                if (path.parent.count()){
                    //let's check if our target still is in same place
                    if ((!CirclesColide(robots[0].pos.v[0],
                                      robots[0].pos.v[1], 8.0f, 
                                      path.parent[path.parent.count()- 1].x * 32.0f + 16.0f,
                                      path.parent[path.parent.count()- 1].y * 32.0f + 16.0f,
                                      16.0f))
                        || ((hit)&&(hittic >= 9))){
                        makePath(map, robots, robotCount);
                    }
                    
                }
                else
                    makePath(map, robots, robotCount);
                 
                if ((path.parent.count())&&(!(robots[0].pos.v[2] > radius/2)))
                    nextSquare = Vector3D((path.parent[pathIndex].x * 32.0f) + 16.0f,
                                          (path.parent[pathIndex].y * 32.0f) + 14.0f, 0);
                else 
                    nextRandomSquare(map);
                
            }
            ai_move(42, 10, 2.8f);
           
        } break;
        //#############################
        case 10:{
            
            if (!isRandomSquare){
                        
                if (!CirclesColide(robots[0].pos.v[0],
                                    robots[0].pos.v[1], 8.0f, 
                                    nextSquare.v[0], nextSquare.v[1],
                                    16.0f)){
                    if ((!robots[0].falling)&&(!robots[0].dead)){
                        nextSquare = robots[0].pos;
                        isRandomSquare = false;
                    }
                }
                
                Robot dummy;
                dummy.radius = radius;
                dummy.pos = Vector3D(pos.v[0] + direction.v[0]*radius, pos.v[1] + direction.v[1]*radius, 0);
                if (dummy.shouldFall(map)){
                    nextRandomSquare(map);
                    isRandomSquare = true;
                }
            }
                
            if (CirclesColide(pos.v[0], pos.v[1], radius/2.0f, nextSquare.v[0], nextSquare.v[1], 4.0f)){
                if ((!robots[0].falling)&&(!robots[0].dead)){
                    nextSquare = robots[0].pos;
                    isRandomSquare = false;
                    
                }
                else{
                    nextRandomSquare(map);
                    isRandomSquare = true;
                }
            }
                    
            
            ai_move(42, 10, 2.8f);
                    
        } break;
        //####################    
        case 11:{ //bomb
            if (timer < ticsUntilBombExplodes){
                timer++;
                frame = (timer % 2 == 0)? 0 : 1;
            }
            else{
                frame = 2;
                if (radius < maxExplosionRadius){
                    if (radius == 13)
                        playAttachedSound(2);
                    growthSpeed = 1.0f;
                    mass += 2;
                }
                else{
                    unsigned x = (unsigned)pos.v[0]/32;
                    unsigned y = (unsigned)pos.v[1]/32;
                    if ((x < map.width) && (y < map.height))
                        map.map[y][x].picture = 0;
                    x++;
                    if ((x < map.width) && (y < map.height))
                        map.map[y][x].picture = (map.map[y][x].picture == 1)? 2 : 0;
                    x-=2;
                    if ((x < map.width) && (y < map.height))
                        map.map[y][x].picture = (map.map[y][x].picture == 1)? 2 : 0;
                    x++;
                    y++;
                    if ((x < map.width) && (y < map.height))
                        map.map[y][x].picture = (map.map[y][x].picture == 1)? 2 : 0;
                    y-=2;
                    if ((x < map.width) && (y < map.height))
                        map.map[y][x].picture = (map.map[y][x].picture == 1)? 2 : 0;
                    
                    growthSpeed = 0;
                    dead = true;
                }
                
            }
        }
            
    } 
    
}
//--------------------------------
void Robot::attachBuffer(SoundSystem& ss, unsigned int index, unsigned int place){
    
    if (alIsSource(soundSources[place]))
        alDeleteSources(1, &soundSources[place]);
    alGenSources(1, &soundSources[place]);
    alSourcei(soundSources[place], AL_BUFFER, ss.getBuffer(index));
    
    ALenum r=0;
    r=alGetError();
    //if ( r != AL_NO_ERROR){
    //    printf("Error: %x while attaching\n",r);
    //}
   // else
   //     puts("attached");
}
//--------------------------------
void Robot::destroy(){
    path.destroy();
    particles.destroy();
    freeSoundSource(0);
    freeSoundSource(1);
    freeSoundSource(2);
}


//--------------------------------
void Robot::freeSoundSource(unsigned int place){
    if (alIsSource(soundSources[place])){
        alSourceStop(soundSources[place]);
        alDeleteSources(1, &soundSources[place]);
    }
}
//---------------------------------
void Robot::playAttachedSound(unsigned int place, float volume){
    if (alIsSource(soundSources[place])){
        alSource3f(soundSources[place], AL_POSITION, pos.x(), pos.z(), pos.y());
        alSource3f(soundSources[place], AL_DIRECTION, direction.x(), direction.z(), direction.y());
        alSource3f(soundSources[place], AL_VELOCITY, direction.x() * speed, direction.z(), direction.y() * speed);
        alSourcei (soundSources[place], AL_SOURCE_RELATIVE, AL_FALSE);
        alSourcef(soundSources[place], AL_CONE_OUTER_ANGLE, 360.0f);
        alSourcef(soundSources[place], AL_CONE_INNER_ANGLE, 360.0f);
        alSourcef(soundSources[place], AL_MAX_DISTANCE, 32.0f * ((place == 2) ? 10 : 5));
        alSourcef(soundSources[place], AL_REFERENCE_DISTANCE, 32.0f);
        alSourcef(soundSources[place], AL_GAIN, ((place == 2) ? 1.0 : 0.6f)*volume);
        alSourcef(soundSources[place], AL_PITCH, 1.0f);
        alSourcePlay(soundSources[place]);
        //ALenum r = 0;
        //r = alGetError();
        /*if ( r != AL_NO_ERROR){
            printf("Error: %x while playing\n",r);
        }*/
    }
   // else 
   //     puts("bad source");

}


