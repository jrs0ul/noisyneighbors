#ifdef WIN32
    #ifdef  _MSC_VER
        #define _CRT_SECURE_NO_DEPRECATE 1
        #pragma comment(lib,"SDL.lib")
        #pragma comment(lib,"SDLmain.lib")
        #pragma comment(lib,"OpenGl32.lib")
        #pragma comment(lib,"GLU32.lib")
        #pragma comment(lib,"openal32.lib")

        #ifdef _DEBUG
            #pragma comment(lib,"ogg_d.lib")
            #pragma comment(lib,"vorbis_d.lib")
            #pragma comment(lib,"vorbisfile_d.lib")
        #else
            #pragma comment(lib,"ogg.lib")
            #pragma comment(lib,"vorbis.lib")
            #pragma comment(lib,"vorbisfile.lib")
        #endif
    #endif
#endif



#include <ctime>
#include "CppSingleton.h"
#include "Threads.h"
#include "SDLVideo.h"
#include "Utils.h"
#ifdef __APPLE__
#include <limits.h>
#include <unistd.h>

#include <CoreFoundation/CoreFoundation.h>
#endif
//#include <string>
//#include <curl/curl.h>


const int WIDTH = 480;
const int HEIGHT = 320;

bool Works = true;
SDLVideo SDL;
SDL_Joystick *Joy = 0;
int JoyX = 0;
int JoyY = 0;
int MouseX, MouseY; //relative mouse coords
int _MouseX, _MouseY;
unsigned long tick;
//static std::string buffer;


Singleton Game;


void ConfGL(){
    Game.init();
}
//-----------------
void RenderScreen(){
    Game.render();
    glFlush();

    SDL_GL_SwapBuffers( );
}
//-----------------
void Logic(){
    Game.logic();
}
//-----------------
static void  process_events(){
    
    SDL_Event event;

    while( SDL_PollEvent( &event ) ) {

        switch( event.type ) {

        case SDL_KEYDOWN:{

            Game.globalKEY = (char)event.key.keysym.unicode;
            switch( event.key.keysym.sym ) {
                default:{}
            }
        } break;
        case SDL_MOUSEBUTTONUP:{
            Vector3D pos(event.button.x, event.button.y, 0);
            Game.touches.up.add(pos);
            Game.touches.allfingersup = true;
        } break;
        case SDL_MOUSEBUTTONDOWN:{
            Vector3D pos(event.button.x, event.button.y, 0);
            Game.touches.down.add(pos);
            Game.touches.allfingersup = false;

        } break;
        case SDL_MOUSEMOTION:{
            if(SDL_GetMouseState(0, 0)&SDL_BUTTON_LMASK){
                Vector3D pos(event.button.x, event.button.y, 0);
                Game.touches.move.add(pos);
                Game.touches.allfingersup = false;
            }
        }break;


        case SDL_QUIT:{
            Works = false;
        }break;
    
        }

    }
}
//--------------------
void checkKeys(){
    Uint8 * keys;
    int JoyNum = 0;
    
    keys = SDL_GetKeyState ( NULL );
    JoyNum = SDL_NumJoysticks();

    if (JoyNum > 0) {

        //printf("%s\n", SDL_JoystickName(0));

        SDL_JoystickUpdate ();
        JoyX = SDL_JoystickGetAxis(Joy, 0);
        JoyY = SDL_JoystickGetAxis(Joy, 1);
    }

    int bm;
    bm = SDL_GetRelativeMouseState ( &MouseX,&MouseY );
    SDL_GetMouseState(&_MouseX, &_MouseY);

    Game.gamepad.v[0] = JoyX/ 1000.0f;
    Game.gamepad.v[1] = JoyY/ 1000.0f;

    //Game.gamepad.v[0] = MouseX * 10.0f;
    //Game.gamepad.v[1] = MouseY * 10.0f;


    memset(Game.Keys, 0, 20);

    if ( keys[SDLK_w] )    Game.Keys[0] = 1;
    if ( keys[SDLK_s] )    Game.Keys[1] = 1;
    if ( keys[SDLK_a] )    Game.Keys[2] = 1;
    if ( keys[SDLK_d] )    Game.Keys[3] = 1;
    if ( keys[SDLK_UP] )   Game.Keys[0] = 1;
    if ( keys[SDLK_DOWN])  Game.Keys[1] = 1;
    if ( keys[SDLK_LEFT])  Game.Keys[2] = 1;
    if ( keys[SDLK_RIGHT]) Game.Keys[3] = 1;
    if ( keys[SDLK_SPACE]) Game.Keys[4] = 1;
    if ( keys[SDLK_RETURN]) Game.Keys[5] = 1;
    if ( keys[SDLK_LCTRL]) Game.Keys[6] = 1;

    if (JoyNum){
        if (SDL_JoystickGetButton (Joy, 0))
            Game.Keys[4] = 1;
        if (SDL_JoystickGetButton (Joy, 1))
            Game.Keys[5] = 1;
    }
}
/*//------------------------
static int writer(char *data, size_t size, size_t nmemb,
                  std::string *buffer){

    int result = 0;
    if (buffer){
            buffer->append(data, size * nmemb);
            result = size * nmemb;
    }

        return result;
}

//----------------------------
THREADFUNC sendScore(void * args){

    Game.getHighScore = true;
    Game.gotHighScore = false;
    Game.failToGetScore = false;
    Game.waitForOnlineScoreTics = 0;
    strcpy(Game.highScoreFromServer,"");
    buffer = "";



    CURLcode result;
    CURL * cu = 0;

    cu = curl_easy_init();
    curl_easy_setopt( cu, CURLOPT_POST, 1);

    char request[1024];
    sprintf(request,"%s0aecf271fc6%d76%d1cb%d909fdd5a839a2",
            Game.highscorePostParams, 2*2, 8/2, 1+1);
    puts(request);

    curl_easy_setopt( cu, CURLOPT_POSTFIELDSIZE, strlen(request) );

    curl_easy_setopt( cu, CURLOPT_POSTFIELDS, request ) ;

    char url[255];
    sprintf(url, "http://jrs0ul.com/score/BumpyRobotsPC.php?mode=add&type=%d", Game.onlineScoreType);
    curl_easy_setopt(cu, CURLOPT_URL, url);
    curl_easy_setopt(cu, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(cu, CURLOPT_WRITEFUNCTION, writer);
    curl_easy_setopt(cu, CURLOPT_WRITEDATA, &buffer);

    result = curl_easy_perform(cu);
    if (cu)
        curl_easy_cleanup(cu);

    return 0;
}
//-------------------------
THREADFUNC getScore(void * args){


    Game.getHighScore = true;
    Game.gotHighScore = false;
    Game.failToGetScore = false;
    Game.waitForOnlineScoreTics = 0;
    strcpy(Game.highScoreFromServer,"");
    buffer = "";
    

    CURLcode result;
    CURL * cu = 0;

    cu = curl_easy_init();

    char url[255];
    sprintf(url, "http://jrs0ul.com/score/BumpyRobotsPC.php?mode=get&type=%d", Game.onlineScoreType);
    
    curl_easy_setopt(cu, CURLOPT_URL, url);
    curl_easy_setopt(cu, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(cu, CURLOPT_WRITEFUNCTION, writer);
    curl_easy_setopt(cu, CURLOPT_WRITEDATA, &buffer);
    
    result = curl_easy_perform(cu);
    
    if (strlen(buffer.c_str()) > 0){
        strncpy(Game.highScoreFromServer, buffer.c_str(), 2024);
        Game.gotHighScore = true; 
    }

    if (cu)
        curl_easy_cleanup(cu);

    return 0;
}
*/



//--------------------
int main( int   argc, char *argv[] ){
    
    srand(time(0));


    char buf[255];
    GetHomePath(buf);
    sprintf(Game.DocumentPath, "%s.NNeighbors", buf);
    MakeDir(Game.DocumentPath);
#ifdef __APPLE__
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX)){
        puts("Um...Error?");
    }
    CFRelease(resourcesURL);
    chdir(path);
#endif

    SDL.setMetrics(WIDTH, HEIGHT);
    if (!SDL.InitWindow("Noisy Neighbors", "icon.bmp")){
        Works = false;
    }

    SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    if(SDL_NumJoysticks() > 0){
        Joy = SDL_JoystickOpen(0);
    }
    SDL_EnableUNICODE(SDL_ENABLE);
    ConfGL();
    //LoadExtensions();
    

     while (Works){
        if ((SDL_GetTicks() > tick)){

            Logic();
            tick = SDL_GetTicks() + 13;
        }
        SDL_Delay(1);

        if (Game.sendScore){
            /*Thread t;
            t.create(&sendScore, 0);
            Game.sendScore = false;*/
        }

        if (Game.getHighScore){
           /* Thread t;

            t.create(&getScore, 0);
            Game.getHighScore = false;*/
        }

        RenderScreen();
        process_events();
        checkKeys();
    }

    Game.destroy();

    SDL.Quit();


    return 0;
}

