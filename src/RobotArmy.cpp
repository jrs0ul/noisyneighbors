#include <cmath>
#include "RobotArmy.h"
#include "Utils.h"
#include "wchar.h"


void    RobotArmy::respawn(unsigned index, LevelMap& map){
    
    if (index < robots.count()){
        Robot * r = &robots[index];
    
        r->pos = Vector3D(-20, -20, 0);
        
        while (r->shouldFall(map)){
            int mwidth = map.width - 3;
            int mheight = map.height - 3;
            r->pos = (!index) ? (Vector3D((((rand() % mwidth)+2)* 32 + 16),((rand() % mheight)+2) * 32 + 16, 0 )):
                                (Vector3D((rand()% map.width)* 32 + 16, (rand()% map.height) * 32 + 16, 0 ));
        }
        r->direction = Vector3D(0,0,0);
        r->speed = 0;
       
        r->dead = false;
        r->falling = 0;
        r->jumping = 0;
        r->mass = 1.0f;
        r->hit = false;
        r->hittic = 0;
        r->maxKnockDownTime = 8;
        r->bombs = initialBombCount;
        r->maxJumpHeight = r->radius * 1.9f;
        int i = 0;
    
        while ((unsigned)i < robots.count()) {
            if ((unsigned)i != index){
            
                Robot * r2 = &robots[i];
                if (CirclesColide(r->pos.v[0], r->pos.v[1], r->radius,
                                  r2->pos.v[0], r2->pos.v[1], r2->radius)){
                
                    r->pos = Vector3D(-20, -20, 0);
                    while (r->shouldFall(map)){
                            int mwidth = map.width - 3;
                            int mheight = map.height - 3;
                            r->pos = (!index) ? (Vector3D((((rand() % mwidth)+2)* 32 + 16),
                                                          ((rand() % mheight)+2) * 32 + 16, 0 )):
                            (Vector3D((rand()% map.width)* 32 + 16, (rand()% map.height) * 32 + 16, 0 ));
                        }                    i = -1;
                }
            }
            i++;
        }
    }
    
}
//------------------------------------------
void RobotArmy::create(LevelMap& map){
    
    unsigned crystals = 0;
    unsigned assaulters = 0;
    unsigned spikeys = 0;
    unsigned astarers = 0;
    
    for (unsigned i = 0; i < 10; i++){

        Robot  r;
        
        r.face = (!i)? 1 : ((rand()%2 == 1)? (4 + rand()% 3): 10);
        switch (r.face){
            case 4 : {
                if (crystals > 5){
                    r.face = 6;
                    astarers++;
                }
                else
                    crystals++;
                
            } break;
            case 5 : {
                if (spikeys > 3){
                    r.face = 10;
                    assaulters++;
                }
                else
                    spikeys++;
            } break;
            case 6 : astarers++; break;
            case 10:{
                if (assaulters > 4){
                    r.face = 4;
                    crystals++;
                }
                else
                    assaulters++;
            }; break;
        }
        
        r.radius = (!i) ? 16 : rand() % 10 + 10;
        r.pos = Vector3D(-20, -20, 0);
        r.c = (r.face == 4) ? COLOR((rand()%100)/100.0f, 0.05f, (rand()%50 + 50)/100.0f, 1) :
                              ((!i)? COLOR(1,1,1):COLOR((rand()%50+50)/100.0f, (rand()%100)/100.0f, 0.05, 1)); 
        while (r.shouldFall(map))
            r.pos = (!i)?Vector3D(((rand()% (map.width - 3))+2) * 32 + 16,
                                  ((rand()% (map.height - 3))+2) * 32 + 16, 0 ):
                                  Vector3D(((rand()% map.width)* 32 + 16),
                                           ((rand()% map.height)) * 32 + 16, 0 );
        r.direction = Vector3D(0,0,0);
        
        r.mass = r.radius/16.0f;
        r.dead = false;
        int index = 0;
        
        
        while ((unsigned)index < robots.count()) {
            if ((unsigned)index != i){
                
                Robot * r2 = getRobot(index);
                if (CirclesColide(r.pos.v[0], r.pos.v[1], r.radius,
                                  r2->pos.v[0], r2->pos.v[1], r2->radius)){
                   
                    r.pos = Vector3D(-20, -20, 0);
                    while (r.shouldFall(map))
                        r.pos = (!i)?Vector3D(((rand()% (map.width - 3))+2) * 32 + 16,
                                              ((rand()% (map.height - 3))+2) * 32 + 16, 0 ):
                        Vector3D(((rand()% map.width)* 32 + 16),
                                 ((rand()% map.height)) * 32 + 16, 0 );
                    index = -1;
                }
            }
            index++;
        }
        
        r.nextSquare = r.pos;
        r.bombs = initialBombCount;
        r.maxJumpHeight = r.radius * 1.9f;
        robots.add(r);
    }
}
//------------------------------
void RobotArmy::loadFromXMLNode(XmlNode * node){
    char buf[100];
    for (unsigned long i = 0; i < node->childrenCount(); i++){
        Robot r;
        memset(r.soundSources, 0 , sizeof(unsigned)*10);
        r.dead = false;
        r.direction = Vector3D(0,0,0);
        
        XmlNode * entity = 0;
        entity = node->getNode(i);
        wcstombs(buf, entity->getAttribute(0)->getValue(), 100);
        r.face = atoi(buf);
        wcstombs(buf, entity->getAttribute(1)->getValue(), 100);
        r.pos.v[0] = atof(buf);
        wcstombs(buf, entity->getAttribute(2)->getValue(), 100);
        r.pos.v[1] = atof(buf);
        wcstombs(buf, entity->getAttribute(3)->getValue(), 100);
        r.radius = atof(buf);
        wcstombs(buf, entity->getAttribute(4)->getValue(), 100);
        r.mass = atof(buf);
        wcstombs(buf, entity->getAttribute(5)->getValue(), 100);
        r.c.c[0] = atof(buf);
        wcstombs(buf, entity->getAttribute(6)->getValue(), 100);
        r.c.c[1] = atof(buf);
        wcstombs(buf, entity->getAttribute(7)->getValue(), 100);
        r.c.c[2] = atof(buf);
        r.nextSquare = r.pos;
        r.maxJumpHeight = r.radius * 1.9f;
        
        robots.add(r);
    }
}
//------------------------------
void RobotArmy::putToXMLNode(XmlNode * node){
    node->setName(L"Entities");
    for (unsigned long i = 0; i < robots.count(); i++){
        XmlNode robot;
        wchar_t buf[100];
        robot.setName(L"Entity");
        swprintf(buf,
                #ifndef __MINGW32__ 
                100,
                #endif
                L"%u", robots[i].face);
        robot.addAtribute(L"kind", buf);
        swprintf(buf,
                #ifndef __MINGW32__ 
                100,
                #endif
                L"%.3f", robots[i].pos.v[0]);
        robot.addAtribute(L"x", buf);
        swprintf(buf,
                #ifndef __MINGW32__ 
                100,
                #endif
                L"%.3f", robots[i].pos.v[1]);
        robot.addAtribute(L"y", buf);
        swprintf(buf,
                #ifndef __MINGW32__ 
                100,
                #endif
                L"%.3f", robots[i].radius);
        robot.addAtribute(L"radius", buf);
        swprintf(buf,
                #ifndef __MINGW32__ 
                100,
                #endif
                L"%.3f", robots[i].mass);
        robot.addAtribute(L"mass", buf);
        swprintf(buf,
                #ifndef __MINGW32__ 
                100,
                #endif
                L"%.3f", robots[i].c.c[0]);
        robot.addAtribute(L"r", buf);
        swprintf(buf,
                #ifndef __MINGW32__ 
                100,
                #endif
                L"%.3f", robots[i].c.c[1]);
        robot.addAtribute(L"g", buf);
        swprintf(buf,
                #ifndef __MINGW32__ 
                100,
                #endif
                L"%.3f", robots[i].c.c[2]);
        robot.addAtribute(L"b", buf);
        node->addChild(robot);
    }
}

//-----------------------------
void RobotArmy::draw(PicsContainer& pics, int x, int y, float c,
                     int screenWidth, int screenHeight){
    
    if (robots.count()){
        
        if ((!robots[0].dead) && (robots[0].falling))
            robots[0].draw(pics, x, y, c, screenWidth, screenHeight);
        
        for (unsigned long i = 1; i < robots.count(); i++){
            if (!robots[i].dead)
                robots[i].draw(pics, x, y, c, screenWidth, screenHeight);
        }
        if ((!robots[0].dead) && (!robots[0].falling))
            robots[0].draw(pics, x, y, c, screenWidth, screenHeight);
        
        for (int i = (int)((int)robots.count())-1; i >= 0; i--){
            if (!robots[(unsigned long)i].dead)
                robots[(unsigned long)i].particles.drawParticles(pics, 7, Vector3D(x, 0, y));
        }
        
    }
}
//--------------------------------------
bool RobotArmy::isHittingSomething(unsigned long index){

    Robot * r = 0;
    r = getRobot(index);
    if (!r)
        return false;
    for (unsigned long i = 0; i < robots.count(); i++){
        if ((index != i) && (!robots[i].dead)){
            Robot * r2 = getRobot(i);
            if (CollisionSphere_Sphere(Vector3D(r->pos.v[0]+(r->direction.v[0] * r->speed),
                                                r->pos.v[1]+(r->direction.v[1] * r->speed),
                                                r->pos.v[2]+(r->direction.v[2] * r->speed)),
                                       r->radius,
                                       r2->pos,
                                       r2->radius)){
                return true;
            }
                
        }
    }
    return false;
}
//-----------------------------------
int RobotArmy::whoHitsIt(unsigned long index, unsigned long * obstacles){
    int obstaclesCount = 0;
    Robot * r = 0;
    r = getRobot(index);
    if (!r)
        return 0;
    for (unsigned long i = 0; i < robots.count(); i++){
        if ((index != i) && (!robots[i].dead)){
            Robot * r2 = getRobot(i);
            if (CollisionSphere_Sphere(r->pos,
                                       r->radius + r->growthSpeed,
                                       Vector3D(r2->pos.v[0]+(r2->direction.v[0] * r2->speed),
                                                r2->pos.v[1]+(r2->direction.v[1] * r2->speed),
                                                r2->pos.v[2]+(r2->direction.v[2] * r2->speed)),
                                       r2->radius + r2->growthSpeed)){
                obstacles[obstaclesCount] = i;
                obstaclesCount++;
                
            }
            
        }
    }
    return obstaclesCount;
}
//-----------------------------------
void RobotArmy::remove(unsigned long index){
    if (index < robots.count()){
        robots[index].destroy();
        robots.remove(index);
    }
}
//------------------------------------
bool RobotArmy::colideWithPowerup(Robot* hitter, Robot* colider, 
                       unsigned hitterIndex, unsigned coliderIndex, float volume){
    
    if (((hitter->face == 12)&&(coliderIndex == 0)) ||
        ((hitterIndex == 0) && (colider->face == 12))){
        
        Robot * whoTakesIt = 0;
        Robot * powerup = 0;
        if (colider->face == 12){
            whoTakesIt = hitter;
            powerup = colider;
        }
        else{
            whoTakesIt = colider;
            powerup = hitter;
        }
        powerup->dead = true;
        whoTakesIt->playAttachedSound(3, volume);
        
        switch(powerup->frame){
            case 0:{
                whoTakesIt->mass += 1;
                
            } break;
            case 1:{
                whoTakesIt->bombs += 1;
                
                
            } break;
            case 2:{
                whoTakesIt->maxJumpHeight += 0.2f;
            
            } break;
        }
        
        return true;

    }
    
    return false;
    
}
//------------------------------------
void RobotArmy::collisionResponse(Robot* r, unsigned long i, float volume){
    unsigned long * indexes = 0;
    indexes = (unsigned long *)malloc(sizeof(unsigned long) * robots.count());
    int colidersCount = 0;
    colidersCount = whoHitsIt(i, indexes);
    
    for (int a = 0; a < colidersCount; a++){
        Robot * hitter = &robots[indexes[a]];//body that hits "r"
        
        if (colideWithPowerup(hitter, r, indexes[a], i, volume))
            break;
                
        Vector3D d(r->direction.v[0] * r->speed*0.5,
                   r->direction.v[1] * r->speed*0.5,
                   r->direction.v[2] * r->speed*0.5);
        Vector3D d1(hitter->direction.v[0] * hitter->speed*0.5,
                    hitter->direction.v[1] * hitter->speed*0.5,
                    hitter->direction.v[2] * hitter->speed*0.5);
    
        Vector3D impulse = r->pos - hitter->pos;
        impulse.normalize();
       
        
        float Js = ((1.5f * hitter->mass * r->mass)/r->mass + hitter->mass)*
                  ((d - d1)*impulse); //dot product
        
        Vector3D J = Vector3D(impulse.v[0]*Js, impulse.v[1]*Js, 0);
        
        Vector3D Va = Vector3D(d.v[0] - J.v[0]/r->mass,
                               d.v[1] - J.v[1]/r->mass, 
                               d.v[2] - J.v[2]/r->mass );
        Vector3D Vb = Vector3D(d1.v[0] + J.v[0]/hitter->mass,
                               d1.v[1] + J.v[1]/hitter->mass,
                               d1.v[2] + J.v[2]/hitter->mass);
        
        if (!((hitter->timer >= ticsUntilBombExplodes) && (hitter->face == 11))){
            hitter->speed = Vb.length();
           
            hitter->direction = Vb;
            hitter->direction.normalize();
            //hitter->direction.v[2] = 0;
            hitter->hit = true;
            hitter->hittic = 0;
            hitter->maxKnockDownTime = 2;
            
            if ((r->timer >= ticsUntilBombExplodes) && (r->face == 11)){
                hitter->speed = 5;
                hitter->direction = hitter->pos - r->pos;
                hitter->direction.normalize();
                hitter->maxKnockDownTime = 8;
            }
        }
        
        if (!((r->timer >= ticsUntilBombExplodes) && (r->face == 11))){ //if it's not an exploding bomb
            
            r->speed = Va.length();
            
            r->direction = Va;
            r->direction.normalize();
            //r->direction.v[2] = 0;
            
            r->hit = true;
            r->hittic = 0;
            r->maxKnockDownTime = 2;
            
            if ((hitter->timer >= ticsUntilBombExplodes) && (hitter->face == 11)){
                r->speed = 5;
                r->direction = r->pos - hitter->pos;
                r->direction.normalize();
                r->maxKnockDownTime = 8;
            }
            
            r->playAttachedSound(0, volume);
            r->spawnParticles(hitter->direction, hitter->speed);
        }
        
    }//for
    if (indexes)
        free(indexes);
}
//------------------------------------
void RobotArmy::react(LevelMap & map, float volume){
    float friction = 0.09f;
    float gravity = 0.9f;
    
    for (unsigned long i = 0; i < robots.count(); i++){//for each robot
        Robot * r = &robots[i];
        r->particles.updateSystem();
       
        if ((!robots[i].dead)&&(!robots[i].falling)){
            
            r->regainConsciousness();
            collisionResponse(r, i);
            
            if (!isHittingSomething(i)){//can it move ?
                r->pos = r->pos + Vector3D(r->direction.v[0] * r->speed,
                                           r->direction.v[1] * r->speed,
                                           r->direction.v[2] * r->speed);
                
                if (r->pos.v[2] < -0.01f){ //landing
                    r->pos.v[2] = 0.0f;
                    r->direction.v[2] = 0.0f;
                    Vector3D sDir = r->getSpriteDirection();
                    
                    int x = (r->pos.v[0] - sDir.v[0] * (r->radius - 2))/32;
                    int y = (r->pos.v[1] - sDir.v[1] * (r->radius - 2))/32;
                    
                    if ((x >= 0)&&((unsigned)x < map.width)&&(y >= 0)&&((unsigned)y < map.height)){
                        if (map.map[y][x].picture > 0)
                            map.map[y][x].picture = ( map.map[y][x].picture == 1)? 2 : 0; 
                        r->playAttachedSound(1, volume);
                    }
                    
                }
                
                if (r->pos.v[2]  > r->maxJumpHeight){ //end of jump
                    r->jumping = false;
                    r->direction.v[2] = 0.0f;
                }
            }
            
            if (r->growthSpeed > 0)
                r->radius += r->growthSpeed;
        
            if (r->pos.v[2] < 0.01f){ //object not in the air
                if (r->speed >= friction)
                    r->speed -= friction;
                if (r->speed < friction)
                    r->speed = 0.0f;
            
                
                if ((r->shouldFall(map))&&(!r->falling)){
                    r->falling = true;
                    r->playAttachedSound(4, volume);
                }
                
            }
            
            //let's run AI
            if (i)//if not player, cuz 0 is player's index
                r->ai(map, (const Robot*)robots.getData(), robots.count());
                
        }
        
        if (r->jumping)
            r->jump();
        
        if (r->falling){
            r->pos.v[2] -= gravity;
            if (r->pos.v[2] < -robots[i].radius * 4.0)
                r->dead = true;
        }
        else{
            if (r->pos.v[2] > 0.01f){
                Vector3D v(r->direction.v[0] * r->speed,
                           r->direction.v[1] * r->speed,
                           r->direction.v[2] * r->speed);
                Vector3D g(0, 0, -gravity);
                r->direction = v + g;
                r->speed = r->direction.length();
                r->direction.normalize();
            }
        }
    }//for
}
//------------------------------------
void RobotArmy::destroy(){
    for (unsigned long i = 0; i < robots.count(); i++){
        robots[i].destroy();
    }
    robots.destroy();
}


//------------------------------------
Robot * RobotArmy::getRobot(unsigned long index){
    if (index < robots.count())
        return &robots[index];
    return 0;
}
//------------------------------------
int RobotArmy::howManyDead(int _kind){
    int result = 0;
    for (unsigned long i = 0; i < robots.count(); i++){
        if (_kind < 0){
            if (robots[i].dead)
                result++;
        }
        else{
            if ((robots[i].dead)&&(robots[i].face == (unsigned)_kind))
                result++;
        }
    }
    
        
    return result;
}
//-----------------------------------
void RobotArmy::howManyAlive(unsigned long * countArray){
    if (countArray){
        for (unsigned long i = 0; i < robots.count(); i++){
            if (!robots[i].dead){
                (countArray[robots[i].face])++;
                //we'll use [0] as total sum of all
                countArray[0]++;
            }
        }
    }
}
//--------------------------------------
void RobotArmy::add(Robot & r){
    robots.add(r);
}

