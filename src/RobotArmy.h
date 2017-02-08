#ifndef _ROBOT_ARMY_H
#define _ROBOT_ARMY_H

#include "Robot.h"
#include "DArray.h"
#include "Xml.h"


class RobotArmy{
    
    DArray<Robot> robots;
    
    bool colideWithPowerup(Robot* hitter, Robot* colider, 
                           unsigned hitterIndex, unsigned coliderIndex, float volume = 1.0f);
    
    void collisionResponse(Robot* r, unsigned long index, float volume = 1.0f);
    
public:
    void    create(LevelMap& map); //creates randomly
    void    loadFromXMLNode(XmlNode * node);
    void    putToXMLNode(XmlNode * node);
    void    draw(PicsContainer& pics, int x, int y, float c = 1.0f,
                 int screenWidth = 480, int screenHeight = 320);
    bool    isHittingSomething(unsigned long index);
    int     whoHitsIt(unsigned long index, unsigned long * obstacles);
    Robot * getRobot(unsigned long index);
    void    destroy();
    void    react(LevelMap& map, float volume = 1.0f);
    int     howManyDead(int _kind);
    void    howManyAlive(unsigned long * countArray);
    void    remove(unsigned long index);
    unsigned long count(){return robots.count();}
    void    add(Robot & r);
    void    respawn(unsigned index, LevelMap& m);
};

#endif //ROBOT_ARMY_H
