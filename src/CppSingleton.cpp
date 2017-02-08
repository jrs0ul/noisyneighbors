#include <wchar.h>
#include "CppSingleton.h"
#include "Utils.h"
#include "gui/Text.h"



void Singleton::setMusicVolume(float vol){
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
    MusicVolume = vol;
    ChangeVolume =true;
#else
    music.setVolume(vol);
#endif
}
//------------------------
void Singleton::playMusic(){
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
    PlayMusic = true;
#else
    music.playback();
#endif
}
//---------------------
void Singleton::playNewSong(const char * path){
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
    PlayNewSong = true;
    sprintf(songName, "data/%s.m4a", path);
#else
    music.stop();
    music.release();
    sprintf(songName, "data/%s.ogg", path);
    music.open(songName);
    music.playback();
#endif
    
}

//---------------------

void Singleton::init(){
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
    glViewport(0, 0, 320, 480);
    glRotatef(-90, 0, 0, 1);
    glOrthof(0.0, (GLfloat) 480, (GLfloat) 320, 0.0, 400, -400);
#else
    glOrtho(0.0, (GLfloat) 480, (GLfloat) 320, 0.0, 400, -400);
#endif
    
        
    if (!pics.load("data/pics/list.txt"))
        puts("can't find list");
    
    ss.init(0);
    Vector3D p(0,0,0);
    ss.setupListener(p.v, p.v);
    ss.loadFiles("data/sfx/", "audio.txt");
#if !(TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
    music.open("data/music.ogg");
#endif
    
    char megabuf[255];
    sprintf(megabuf, "%s/settings.cfg", DocumentPath);
    sys.load(megabuf);

    sprintf(megabuf, "%s/score.dat", DocumentPath);
    highscores.load(megabuf, 10);
    
    strcpy(nameToEnter, sys.nameToEnter);
    
    joystick.pos = sys.joystick;
    
    setMusicVolume(sys.musicVolume);
    ss.setVolume(sys.sfxVolume);
    playMusic();
   
    for (unsigned i = 0; i < 2; i++){
        if (alIsSource(Fatsos[i].buffer))
            alDeleteSources(1, &Fatsos[i].buffer);
        alGenSources(1, &Fatsos[i].buffer);
        alSourcei(Fatsos[i].buffer, AL_BUFFER, ss.getBuffer(2));
    }
    
    
}
//----------------------------
void Singleton::drawGame(){
    pics.draw(0, 0, 0);

    OldFart.draw(pics, 1, 10);

    for (unsigned i = 0; i < 2 ; i++){
        if (!Fatsos[i].dead)
            Fatsos[i].draw(pics, 2);
    }

    TheDoor.draw(pics, 4);

    for (unsigned i = 0; i < 10 ; i++){
        if (!Kids[i].dead)
            Kids[i].draw(pics, 3);
    }
    for (unsigned i = 0; i < 10 ; i++){
        if (!Gogies[i].dead)
            Gogies[i].draw(pics, 5);
    }

    if (!GameOver)
        Projectiles.draw(pics);
    


    pics.draw(-1, 5, 5, 0, false, OldFart.hp, 20, 0, COLOR(1,0,0,0.7f), COLOR(0.5,0,0,0.7f));
    WriteShadedText(10, 8, pics, 16, "Life");


    if (GameOver){
        pics.draw(9, 240, -20 + GameOverTics, 0, true);
        highscores.display(pics, 16, 10, 165, 160);
    }

    char buf[100];
    sprintf(buf, "%ld", TheScore);
    pics.draw(14, 400, 5, 0, false, 0.7f, 0.7f);
    for (unsigned z = 0; z < strlen(buf); z++)
        pics.draw(13, 400+z*20, 30, buf[z] - 48, false, 0.6f, 0.6f);


    pics.drawBatch(666);
}
//----------------------------
void Singleton::resetGame(){
        OldFart.reset();
        OldFart.pos = Vector3D(240, 215,0);

        TheScore = 0;
        KidsKilled = 0;
        DogsKilled = 0;
        MaxKids = 0;
        MaxDogs = 1;

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

        for (unsigned i = 0; i < 10; i++){
            Gogies[i].reset();
            Gogies[i].dead = true;
        }

        Projectiles.destroy();
}
//----------------------------
void Singleton::projectilesVSEverything(){
    for (unsigned i = 0; i < Projectiles.count(); i++){
        Projectile * p = 0;
        p = Projectiles.get(i);
        if (p){
            if (p->picIndex == 11){
                for (unsigned a = 0; a < 10; a++){
                    if (!Kids[a].dead){

                        if (CirclesColide(Kids[a].pos.v[0] + 32, Kids[a].pos.v[1] + 32, 32,
                                          p->pos.v[0], p->pos.v[1], 8)){

                            p->dead = true;
                            if (Kids[a].hp > 0){
                                Kids[a].hp--;
                                Kids[a].damaged = true;
                                Kids[a].damagetics = 0;
                            }
                            else{
                                Kids[a].killed = true;
                                TheScore+=5;
                                KidsKilled++;
                                if (KidsKilled > MaxDogs){
                                    MaxDogs++;
                                    KidsKilled = 0;
                                }
                                if (MaxDogs > 10)
                                    MaxDogs = 10;
                            }

                        }
                    }
                }//kids;

                if (p->dead)
                    continue;
                for (unsigned a = 0; a < 10; a++){
                    if (!Gogies[a].dead){
                        if (CirclesColide(Gogies[a].pos.v[0] + 32, Gogies[a].pos.v[1] + 22, 22,
                                          p->pos.v[0], p->pos.v[1], 8)){
                            p->dead = true;
                            if (Gogies[a].hp > 0){
                                Gogies[a].hp--;
                                Gogies[a].damaged = true;
                                Gogies[a].damagetics = 0;
                            }
                            else{
                                Gogies[a].killed = true;
                                TheScore+=5;
                                DogsKilled++;
                                if (DogsKilled > MaxKids){
                                    MaxKids++;
                                    DogsKilled = 0;
                                }
                                if (MaxKids > 10)
                                    MaxKids = 10;
                                Gogies[a].sit = false;
                                Gogies[a].run = false;
                                Gogies[a].runtics = 0;
                                Gogies[a].sittics = 0;
                            }

                        }
                    }

                }

                if (p->dead)
                    continue;

                for (unsigned a = 0; a < 2; a++){
                    if ((!Fatsos[a].dead)&&(Fatsos[a].drilling)){
                        if (CirclesColide(Fatsos[a].pos.v[0] + 36, Fatsos[a].pos.v[1] + 50, 25,
                                          p->pos.v[0], p->pos.v[1], 8)){

                            p->dead = true;
                            Fatsos[a].drilling = false;
                            Fatsos[a].restingtics = 0;
                            Fatsos[a].drillingtics = 0;
                            alSourceStop(Fatsos[a].buffer);

                            if (Fatsos[a].hp > 0){
                                Fatsos[a].hp--;
                                Fatsos[a].frame++;
                                Fatsos[a].resting = true;
                            }
                            else{
                                Fatsos[a].killed = true;
                                TheScore+= 10;
                                Fatsos[a].frame = (Fatsos[a].frame > 4) ? 1 : 6;
                            }
                        }
                    }
                }

            }
        }
    }

}
//---------------------------
void Singleton::spawnStuff(){

    int aliveDogs = 0;
    int aliveKids = 0;
    int aliveDrillers = 0;
    for (unsigned i = 0; i < 10; i++){
        if (!Gogies[i].dead)
            aliveDogs++;
    }
    for (unsigned i = 0; i < 10; i++){
        if (!Kids[i].dead)
            aliveKids++;
    }
    for (unsigned i = 0; i < 2; i++){
        if (!Fatsos[i].dead)
            aliveDrillers = 0;
    }

    SpawnTics++;
    if (SpawnTics > 500){
        SpawnTics = 0;
        if (aliveDogs < MaxDogs){
            for (unsigned i = 0; i < 10; i++){
                if (Gogies[i].dead){
                    TheDoor.openIt = true;
                    Gogies[i].reset();
                    return;
                }
            }
        }
        if (aliveKids < MaxKids){
            for (unsigned i = 0; i < 10; i++){

                if (Kids[i].dead){
                    TheDoor.openIt = true;
                    Kids[i].reset();
                    return;
                }
            }
        }
        if (aliveDrillers < 2){
            for (unsigned i = 0; i < 2; i++){
                if (Fatsos[i].dead){
                    Fatsos[i].reset();
                    Fatsos[i].pos = (!i)? Vector3D(-35, 192, 0) : Vector3D(480, 192, 0);
                }
            }
        }
    }
}


//----------------------------
void Singleton::gameLogic(){

    if (!GameOver){
        //----------old man 
        if (Keys[3])
            OldFart.moveRight();
        if (Keys[2])
            OldFart.moveLeft();

        if (Keys[6]){
            if (!OldFart.attacks)
                OldFart.attacks = true;
        }
        OldFart.damageAnim();

        if (OldFart.attacks)
            OldFart.attack(Projectiles, ss);

        if (Keys[4]){
            if ((!OldFart.jump)&&(OldFart.pos.v[1]>=215))
                OldFart.jump = true;
        }

        if (OldFart.jump){
            OldFart.pos.v[1] -= JUMP;
            OldFart.frame = (OldFart.frame > 4) ? 9 : 4;
            OldFart.jumptics++;
            if ((OldFart.jumptics > 16)||(OldFart.pos.v[1]< 134)){
                OldFart.jump = false;
                OldFart.jumptics = 0;
            }
        }
    }
    else{
        if (GameOverTics < 150)
            GameOverTics++;
    }

    if (OldFart.pos.v[1] < 215)
        OldFart.pos.v[1] += GRAVITY;
    else{
        if ((OldFart.frame == 9)||(OldFart.frame == 4))
            OldFart.frame = (OldFart.frame > 4) ? 5 : 0;
    }
    //-------------------------

    for (unsigned i = 0; i < 2; i++){

        Fatsos[i].AI(Projectiles, OldFart, ss);
    }

    TheDoor.AI(Projectiles, OldFart, ss);
   
    //---------------------------
    for (unsigned i = 0; i < 10; i++)
        Kids[i].AI(Projectiles, OldFart, TheDoor, ss);

    for (unsigned i = 0; i < 10; i++)
        Gogies[i].AI(Projectiles, OldFart, TheDoor, ss);
    //-----------
    Projectiles.update(OldFart.pos, OldFart.hp, OldFart.damaged, OldFart.damagetics);
    projectilesVSEverything();
   //-----------


    if (OldFart.hp <= 0){
        if (!GameOver){
            GameOver = true;
            highscores.addScore("me", TheScore, 10);
            char megabuf[255];
            sprintf(megabuf, "%s/score.dat", DocumentPath);
            highscores.write(megabuf, 10);
        }
    }


    //----
    spawnStuff();
    //----


    if (touches.up.count()){
        if ((GameOver)&&(GameOverTics > 100)){
            GameOverTics = 0;
            GameOver = false;
            gamestate = TITLE;
            resetGame();

        }
    }

}
//--------------------------
void Singleton::drawMainMenu(){
    pics.draw(15, 0, 0, 0, false, 480/256.0f, 320/256.0f);
    Play.draw(pics, 17, 0);
    GoScores.draw(pics, 17, 1);
    pics.drawBatch(666);
}
//------------------------
void Singleton::drawScores(){
    pics.draw(15, 0, 0, 0, false, 480/256.0f, 320/256.0f);
    highscores.display(pics, 16, 10, 165, 140);
    pics.drawBatch(666);
}
//--------------------------
void Singleton::scoresLogic(){

    if (touches.up.count()){
        gamestate = TITLE;
    }
}
//--------------------------
void Singleton::mainMenuLogic(){

    COLOR dark(0.5,0.5,0.5);
    COLOR normal(1,1,1);
    if (touches.up.count()){

        float mx =  touches.up[0].v[0];
        float my =  touches.up[0].v[1];

        if (Play.isPointerOnTop(mx, my)){
            Play.c = normal;
            gamestate = GAME;
        }
        if (GoScores.isPointerOnTop(mx, my)){
            GoScores.c = normal;
            gamestate = SCORES;
        }

    }

    if (touches.move.count()){
        float mx =  touches.move[0].v[0];
        float my =  touches.move[0].v[1];
        if (Play.isPointerOnTop(mx, my)){
            Play.c = dark;
        }
        if (GoScores.isPointerOnTop(mx, my)){
            GoScores.c = dark;
        }
    }

    if (touches.down.count()){
        float mx =  touches.down[0].v[0];
        float my =  touches.down[0].v[1];
        if (Play.isPointerOnTop(mx, my)){
            Play.c = dark;
        }
        if (GoScores.isPointerOnTop(mx, my)){
            GoScores.c = dark;
        }
    }

    if (touches.allfingersup){
        Play.c = normal;
        GoScores.c = normal;
    }
}
//-------------------
void Singleton::render(){
    
   
    glClear(GL_COLOR_BUFFER_BIT);
    switch(gamestate){
        case TITLE : drawMainMenu(); break;
        case GAME : drawGame(); break;
        case SCORES: drawScores(); break;
    }
    
    
}


//---------------------
void Singleton::logic(){
#if !(TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
    if (music.playing())
        music.update();
#endif
    
    switch(gamestate){
        case TITLE: mainMenuLogic(); break;
        case GAME: gameLogic(); break;
        case SCORES: scoresLogic(); break;
    }
    
    touches.oldDown.destroy();
    for (unsigned long i = 0; i < touches.down.count(); i++ ){
        Vector3D v = touches.down[i];
        touches.oldDown.add(v);
    }
    touches.up.destroy();
    touches.down.destroy();
    touches.move.destroy();
}
//-------------------------
void Singleton::destroy(){
    
    touches.up.destroy();
    touches.down.destroy();
    touches.move.destroy();
    touches.oldDown.destroy();
    
#if (!TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
    music.stop();
    music.release();
#endif


    if (alIsSource(Fatsos[0].buffer))
        alDeleteSources(1, &Fatsos[0].buffer);
    if (alIsSource(Fatsos[1].buffer))
        alDeleteSources(1, &Fatsos[1].buffer);
    ss.exit();
    
    
    Projectiles.destroy();
    pics.destroy();
}


