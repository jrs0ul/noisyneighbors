#ifndef _CPP_SINGLETON
#define _CPP_SINGLETON


#include "audio/SoundSystem.h"
#include "audio/OggStream.h"
#include "TextureLoader.h"
#include "Vectors.h"
#include "SysConfig.h"
#include "TouchData.h"
#include "HighScore.h"
#include "externals.h"
#include "OldMan.h"
#include "Driller.h"
#include "Brat.h"
#include "Door.h"
#include "gui/Button.h"
#include "Dog.h"


enum GameStates {TITLE, GAME, SCORES};


struct Joystick{
    Vector3D pos;
    float radius;
    
    Joystick(){
        pos = Vector3D(70, 250, 0);
        radius = 50.0;
    }
};


class Singleton{
    
    void setMusicVolume(float vol);
    void playMusic();
    void playNewSong(const char* path);

    void drawMainMenu();
    void drawGame();
    void mainMenuLogic();
    void gameLogic();
    void resetGame();
    void projectilesVSEverything();
    void spawnStuff();
    void scoresLogic();
    void drawScores();

     
public:
    bool Exit;
    
    bool ChangeVolume;
    float MusicVolume;
    bool PlayMusic;
    bool PlayNewSong;
    bool StopMusic;


    bool GameOver;
    int GameOverTics;
    int SpawnTics;

    long TheScore;
    
    
    SoundSystem ss;
    OggStream music;
    
    PicsContainer pics;
    
    
    bool getHighScore; //fetch me some scores!
    bool gotHighScore; //got some xml shit
    bool sendScore;   //send my scores quickly
    bool failToGetScore; 
    int onlineScoreType; //what type of scores
    char highscorePostParams[255]; //post parameters to send
    int waitForOnlineScoreTics; //wait tics until timeout
    char highScoreFromServer[2024]; //xml thingie
    bool submitOnline; //do I have to send my scores online?
    
    SystemConfig sys;
    
    Vector3D accelerometer;
    Vector3D pos;
    Vector3D gamepad; //input from real device
   
    int MaxKids;
    int MaxDogs;
    int DogsKilled;
    int KidsKilled;


    Button Play;
    Button GoScores;

    
    OldMan OldFart;

    Driller Fatsos[2];
    Brat    Kids[10];
    Dog     Gogies[10];
    ProjectileList Projectiles;

    Door TheDoor;


    char DocumentPath[255];
    
    char songName[255];
    

    HighScores highscores;
    
    Joystick joystick;
    TouchData touches;
    unsigned char Keys[20];
    float acc;
    GameStates gamestate;
    
    bool fadeOut;
    float alpha;
    
    bool levelCreated;
    
    bool joyPressed;
    
    char nameToEnter[256];
    
    bool ActivateEditField;
    bool TextEntered;
    unsigned globalKEY;
    //---
    
    Singleton(){
        gamestate = TITLE;
        Exit = false;
        alpha = 0.0f;
        fadeOut = false;
        joyPressed = false;
        ActivateEditField = false;
        TextEntered = false;
        acc = 24.0f;
        KidsKilled = 0;
        DogsKilled = 0;

        SpawnTics = 0;
        MaxKids = 0;
        MaxDogs = 1;
        
        getHighScore = false;
        gotHighScore= false;
        waitForOnlineScoreTics = 0;
        onlineScoreType = 0;
        sendScore = false;
        submitOnline = false;
        failToGetScore = false;
        
        ChangeVolume = false;
        MusicVolume = 0.0f;
        PlayMusic = false;
        StopMusic = false;
        PlayNewSong = false;

        TheScore = 0;


        GameOver = false;
        GameOverTics = 0;

       //--- 
        OldFart.pos = Vector3D(240, 215,0);

        //---

        Fatsos[0].reset();
        Fatsos[0].dead = true;
        Fatsos[0].pos = Vector3D(-35, 192, 0);
        Fatsos[0].frame = 1;
        Fatsos[1].reset();
        Fatsos[1].dead = true;
        Fatsos[1].pos = Vector3D(480, 192, 0);
        Fatsos[1].frame = 5;

        for (unsigned i = 0; i < 10; i++){
            Kids[i].reset();
            Kids[i].dead = true;
        }

        TheDoor.pos = Vector3D(122, -1, 0);
        TheDoor.open = false;
        TheDoor.frame = 0;

        for (unsigned i = 0; i < 10; i++)
            Gogies[i].dead = true;
        //Gogies[0].pos = Vector3D(150, 72, 0);
        

        Play.set(10, 180, 128, 64);
        GoScores.set(10, 250, 128, 64);
    
    }
    
    void init();
    void logic();
    void render();
    void destroy();
    
};





#endif //_CPP_SINGLETON

