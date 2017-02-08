#ifndef _ROBOT_H
#define _ROBOT_H

#include "Vectors.h"
#include "TextureLoader.h"
#include "LevelMap.h"
#include "FindPath.h"
#include "Particles2D.h"
#include "externals.h"
#include "audio/SoundSystem.h"

class Robot{
    Vector3D spriteDirection;
    float spriteAngle;
public:
    Vector3D pos;
    Vector3D direction;
    
    bool     isRandomSquare; //for AI
    Vector3D nextSquare; //for AI
    Path     path; //for AI too
    int      pathIndex;
    
    Particle2DSystem particles;
    
    float    mass;
    float    speed;
    float    radius;
    float    growthSpeed;
    unsigned face; 
    unsigned frame;
    COLOR    c;
    
    bool     dead;
    bool     falling;
    bool     jumping;
    float    maxJumpHeight;
    
    bool     hit;
    int      hittic;
    int      maxKnockDownTime;
    
    int      timer;
    
    int     bombs;
    
    unsigned soundSources[10];
    
    
    void ai_move(int maxSpeedGain, int minSpeedGain, float maxSpeed);
    void makePath(LevelMap& map, const Robot * robots, unsigned long robotCount);
    void nextRandomSquare(LevelMap& map);
    void spawnParticles(Vector3D hitterdir, float strength);
    
    
    Robot(){
        dead = false; falling = false; jumping = false;
        hit = false; hittic = 0; pathIndex = 0;
        maxKnockDownTime = 10; timer = 0;
        frame = 0; growthSpeed = 0;
        isRandomSquare = false;
        bombs = initialBombCount;
        maxJumpHeight = 16.0f * 2.5f;
        spriteAngle = 0;
    }
    Vector3D getSpriteDirection(){return spriteDirection;}
    void setSpriteDirection(Vector3D dir);
    void draw(PicsContainer& pics, int x, int y, float _c = 1.0f,
              int screenWidth = 480, int screenHeight = 320);
    void jump();
    void regainConsciousness();
    bool shouldFall(LevelMap& map);
    void ai(LevelMap& map, const Robot* robots, unsigned long robotCount);
    void destroy();
    
    void attachBuffer(SoundSystem& ss, unsigned int index, unsigned int place);
	void freeSoundSource(unsigned int place);
	void playAttachedSound(unsigned int place, float volume = 1.0f);
    
    
    
};



#endif //_ROBOT_H
