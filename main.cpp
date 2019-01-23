
#include <windows.h>

#include <SDL.h>
#include "bass.h"

#undef min
#undef max
#include "Utils.h"
#include "GLUtils.h"

#include <stdio.h>
#include <stdlib.h>

void quit() { SDL_Quit(); exit(0); }

enum Key { KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN, KEY_ENTER, KEY_ESC, KEY_R, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_COUNT };

bool keyDown[KEY_COUNT];

bool eventQuit = false;

void processEvents()
{
    for(int key=0;key<KEY_COUNT;key++)
    {
        keyDown[key] = false;
    }

    SDL_Event event;

    while(SDL_PollEvent(&event)) 
    {
        switch(event.type) 
        {
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
                case SDLK_UP:     keyDown[KEY_UP]    = true; break;
                case SDLK_DOWN:   keyDown[KEY_DOWN]  = true; break;
                case SDLK_RIGHT:  keyDown[KEY_RIGHT] = true; break;
                case SDLK_LEFT:   keyDown[KEY_LEFT]  = true; break;
                case SDLK_RETURN: keyDown[KEY_ENTER] = true; break;
                case SDLK_ESCAPE: keyDown[KEY_ESC]   = true; break;
                case SDLK_r:      keyDown[KEY_R]     = true; break;
                case SDLK_1:      keyDown[KEY_1]     = true; break;
                case SDLK_2:      keyDown[KEY_2]     = true; break;
                case SDLK_3:      keyDown[KEY_3]     = true; break;
                case SDLK_4:      keyDown[KEY_4]     = true; break;
                case SDLK_5:      keyDown[KEY_5]     = true; break;
                case SDLK_6:      keyDown[KEY_6]     = true; break;
                case SDLK_7:      keyDown[KEY_7]     = true; break;
                case SDLK_8:      keyDown[KEY_8]     = true; break;
            }
            break;

        case SDL_QUIT:
            eventQuit = true;
            break;

        default:
            break;
        }
    }
}

void FAIL(char* message)
{
    SDL_Quit();
    exit(1);
}

void SDLInit(int width,int height,bool fullscreen,char* caption)
{
    putenv(strdup("SDL_VIDEO_CENTERED=1"));

    const SDL_VideoInfo* info = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) FAIL("video fail");

    info = SDL_GetVideoInfo( );

    if ( !info ) FAIL("video fail");

    int bpp = 32;

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);

    int flags = SDL_OPENGL;
    
    if (fullscreen) flags |= SDL_FULLSCREEN;

    if (SDL_SetVideoMode(width,height,bpp,flags)==0) FAIL("video fail");
    
    SDL_WM_SetCaption(caption,"");
}

//#include "gui.h"
//#define ID __COUNTER__

#include <vector>
using namespace std;

const int TW = 80; 
const int TH = 60;

const int MW = 10;
const int MH = 10;

unsigned int time()
{
    return SDL_GetTicks();    
}

class Image
{
public:
    Image() : imageWidth(0),imageHeight(0),texture(0) {};
  
    Image(char* filename)
    {
        A2V4uc image = imageRead<V4uc>(filename);
        
        imageWidth = image.width();
        imageHeight = image.height();

        texture = new GLTexture2D();
        texture->setMinFilter(GL_NEAREST)
                .setMagFilter(GL_NEAREST)
                .setWrapS(GL_CLAMP)
                .setWrapT(GL_CLAMP)
                .setImage(image);
    }

    int width() { return imageWidth; }
    int height() { return imageHeight; }
    void bind() { texture->bind(); }

private:
    int imageWidth;
    int imageHeight;
    GLTexture2D* texture;        
};

void drawImage(Image* image,int x,int y)
{
    const int w = image->width();
    const int h = image->height();
    image->bind();    
    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex2f(  x,y  );
        glTexCoord2f(1,0); glVertex2f(x+w,y  );
        glTexCoord2f(1,1); glVertex2f(x+w,y+h);
        glTexCoord2f(0,1); glVertex2f(  x,y+h);
    glEnd();        
}

void drawSprite(Image* image,int x,int y,int xofs,int yofs)
{
    const int w = image->width();
    const int h = image->height();
    image->bind();    
    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex2f(  x*TW+xofs,  y*TH+yofs);
        glTexCoord2f(1,0); glVertex2f(w+x*TW+xofs,  y*TH+yofs);
        glTexCoord2f(1,1); glVertex2f(w+x*TW+xofs,h+y*TH+yofs);
        glTexCoord2f(0,1); glVertex2f(  x*TW+xofs,h+y*TH+yofs);
    glEnd();    
}

void drawBackground(Image* image)
{
    const int w = image->width();
    const int h = image->height();

    for(int x=-1;x<800/w+2;x++)
    for(int y=-1;y<600/h+2;y++)
    {
        drawImage(image,x*w+(time()/80)%w,y*h);
        //drawImage(image,x*w,y*h);
    }
}

enum EntityType 
{ 
    E_NONE = 0,
    E_ENEMY,
    E_ITEM,
    E_GOAL,
    E_ENEMY_NEUTRAL,
    E_COUNT
};

enum PlatformType
{ 
    P_NONE = 0,
    P_ISLAND, 
    P_DINO,
    P_MOVER,
    P_COUNT
};

enum PlayerState { PS_IDLING, PS_MOVING, PS_JUMPING, PS_DYING, PS_WINNING };
enum Direction { DIR_LEFT, DIR_RIGHT, DIR_UP, DIR_DOWN };

enum GameState { GS_MENU, GS_PLAY, GS_FAIL, GS_GREAT };

float smoothstep(float x)
{
    return x*x*x*(x*(x*6.0f - 15.0f) + 10.0f);
}

float bias(float b,float x)
{
    return powf(x,logf(b)/logf(0.5f));
}

struct Mover
{
    int map_y;
    int map_x1;
    int map_x2;
    int state;
    int last_time;
    int x;
    int y;

    int map_x1_init;
    int map_x2_init;
    int wait_duration;
    int slowness;

    Mover(int m_y,int m_x1,int m_x2,int m_wait_duration,int m_slowness) 
    {    
        map_x1_init = m_x1;
        map_x2_init = m_x2;    
        map_y = m_y;
        wait_duration = m_wait_duration;
        slowness = m_slowness;
    }

    void reset()
    {
        state = 0;
        map_x1 = map_x1_init;
        map_x2 = map_x2_init;
        last_time = time();
        y = map_y*TH;
    }

    void update()
    {
        if (state == 0)
        {
            x = map_x1*TW;
            y = map_y*TH;
            if (time()-last_time > wait_duration)
            {
                state = 1;
                last_time = time();
            }
        }
        else if (state == 1)
        {
            if (time()-last_time > slowness*(max(map_x1,map_x2)-min(map_x1,map_x2)))
            {
                state = 0;
                swap(map_x1,map_x2);
                last_time = time();
            }
            else
            {
                x = float(map_x1*TW)+((time()-last_time)/float(slowness))*float(TW)*(map_x1<map_x2 ? 1.0f : -1.0f);
            }            
        }        
    }
};

const int numlevels = 8;
const char* levels[8] = { "data\\level1.txt", 
                          "data\\level2.txt",
                          "data\\level3.txt",
                          "data\\level4.txt",
                          "data\\level5.txt",
                          "data\\level6.txt",
                          "data\\level7.txt",
                          "data\\level8.txt" };

/// GAME STATE
GameState game_state;
int level;
int lives;

/// LEVEL STATE
Array2<int> platformMap;
Array2<int> sinkMap;
Array2<int> entityMap;
Array2<int> moverMap;

vector<Mover> movers;

int sink_cycle_duration;

int level_start_time;
int sink_cycle;


/// PLAYER STATE
int player_x;
int player_y;

int player_map_x;
int player_map_y;

int player_mover = -1;
int player_numitems;

Direction jump_dir;
PlayerState player_state;

////
const int player_x_ofs = 0;
const int player_y_ofs = -35;


void skipComments(FILE* f)
{
    while(1)
    {
        char c = fgetc(f);
        if (c=='\n') { continue; }
        else if (c=='#')
        {     
          while(1) { c=fgetc(f); if (c=='\n' || c==EOF) break; }  
        }
        else
        {
            ungetc(c,f);
            break;
        }
    }    
}

void readMap(FILE* f,Array2<int>& map)
{
    for(int y=0;y<10;y++)    
    {
        for(int x=0;x<10;x++)
        {
            fscanf(f,"%d",&map(x,y));
        }
    }    
}

void readMovers(FILE* f)
{
    movers.clear();

    int num_movers;
    fscanf(f,"%d",&num_movers);

    for(int i=0;i<num_movers;i++)
    {
        int Y,X1,X2,WAIT_TIME,SLOWNESS;
        fscanf(f,"%d",&Y);
        fscanf(f,"%d",&X1);
        fscanf(f,"%d",&X2);
        fscanf(f,"%d",&WAIT_TIME);
        fscanf(f,"%d",&SLOWNESS);

        movers.push_back(Mover(Y,X1,X2,WAIT_TIME,SLOWNESS));
    }
}

void loadLevel(char* filename)
{
    FILE* f;

    f = fopen(filename,"r");

    platformMap = Array2<int>(10,10);
    sinkMap = Array2<int>(10,10);
    entityMap = Array2<int>(10,10);
    moverMap = Array2<int>(10,10);

    skipComments(f);
    fscanf(f,"%d",&player_map_x);
    fscanf(f,"%d",&player_map_y);
    skipComments(f);
    fscanf(f,"%d",&sink_cycle_duration);
    skipComments(f);
    readMap(f,platformMap);
    skipComments(f);
    readMap(f,sinkMap);
    skipComments(f);
    readMap(f,entityMap);
    skipComments(f);
    readMap(f,moverMap);
    skipComments(f);
    readMovers(f);
    fclose(f);
}


void levelRestart()
{
    loadLevel((char*)levels[level]);
    
    player_x = player_map_x*TW+player_x_ofs;
    player_y = player_map_y*TH+player_y_ofs;
    
    player_state = PS_IDLING;    

    for(int i=0;i<movers.size();i++) movers[i].reset();

    level_start_time = time();

    player_numitems = 0;
//    player_sprite = player_spr_idle[0]; 
}

int sinkStage(int x,int y)
{
    if (platformMap(x,y)==P_NONE) return 5;
    if (sinkMap(x,y)==0) return 0;

    int stage = sinkMap(x,y)-sink_cycle;
    if      (stage > 4)  stage = 0;
    else if (stage >= 0) stage = 4-stage;
    else stage = 5;

    return stage;
}

int fadein_start_time;

void fadein(float r=0,float g=0,float b=0,float duration=500.0f)
{  
    float alpha = 1.0f-float(time()-fadein_start_time)/duration;
    if (alpha > 0.0f)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        
        glBegin(GL_QUADS);
            glColor4f(r,g,b,alpha);
            glVertex2f(  0,  0);
            glColor4f(r,g,b,alpha);
            glVertex2f(800,  0);
            glColor4f(r,g,b,alpha);
            glVertex2f(800,600);
            glColor4f(r,g,b,alpha);
            glVertex2f(  0,600);
            glColor4f(1,1,1,1);
        glEnd();
        glDisable(GL_BLEND);
    }
}

void blendquad(float r,float g,float b,float a)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUADS);
        glColor4f(r,g,b,a);
        glVertex2f(  0,  0);
        glColor4f(r,g,b,a);
        glVertex2f(800,  0);
        glColor4f(r,g,b,a);
        glVertex2f(800,600);
        glColor4f(r,g,b,a);
        glVertex2f(  0,600);
        glColor4f(1,1,1,1);
    glEnd();
    glDisable(GL_BLEND);    
}

void playSound(HSAMPLE sample)
{
    BASS_ChannelPlay(BASS_SampleGetChannel(sample,FALSE),FALSE);
}

int main(int argc,char* argv[])
{
    SDLInit(800,600,true,"SWAMP RUN");
    
    SDL_ShowCursor(0);

    BASS_Init(-1,44100,0,0,NULL);  
 
    Image* player_sprite;

    Image* player_spr_idle[2] =  { new Image("data\\s-player1.png"), new Image("data\\s-player2.png") };
    Image* player_spr_moving[2] =  { new Image("data\\s-player3.png"), new Image("data\\s-player4.png") };
    Image* player_spr_death[2] = { new Image("data\\s-player5.png"), new Image("data\\s-player6.png") };

    Image* bkg[4] = { new Image("data\\water1.png"), 
                      new Image("data\\water2.png"),
                      new Image("data\\water3.png"),    
                      new Image("data\\water4.png") };

    Image* startScreen = new Image("data\\startscreen.png");
    Image* buttonRun   = new Image("data\\button-run.png");
    Image* buttonExit  = new Image("data\\button-exit.png");

    Image* failScreen = new Image("data\\failscreen.png");
    Image* greatScreen = new Image("data\\okscreen.png");

    Image* islandImages[5] = { new Image("data\\s-isle1.png"),
                               new Image("data\\s-isle2.png"),
                               new Image("data\\s-isle3.png"),
                               new Image("data\\s-isle4.png"), 
                               new Image("data\\s-isle5.png") };

    Image* dinoImages[5] = { new Image("data\\s-dino-a1.png"),
                             new Image("data\\s-dino-a2.png"),
                             new Image("data\\s-dino-a3.png"),
                             new Image("data\\s-dino-a4.png"), 
                             new Image("data\\s-dino-a5.png") };

    Image* moverImage = new Image("data\\s-dino2.png");    

    
    Image* entityImage[E_COUNT];
    entityImage[E_NONE] = 0;
    entityImage[E_ENEMY] = new Image("data\\s-enemy.png");
    entityImage[E_ENEMY_NEUTRAL] = new Image("data\\s-enemy-death.png");
    entityImage[E_ITEM] = new Image("data\\s-apple.png");
    entityImage[E_GOAL] = new Image("data\\s-end.png");    

    int entityOffset[E_COUNT];
    entityOffset[E_NONE] = 0;
    entityOffset[E_ENEMY] = -TH/2;
    entityOffset[E_ITEM] = -TH/4;
    entityOffset[E_GOAL] = -8;
    entityOffset[E_ENEMY_NEUTRAL] = -TH/2;

    int sinkOffset[6] = { 0, 8, 16, 22, 24, 24 };

    BASS_Init(-1, 44100, 0, 0, NULL);  
    
    HSAMPLE sound_jump = BASS_SampleLoad(FALSE,"data\\jump.wav",0,0,3,BASS_SAMPLE_OVER_POS);
    HSAMPLE sound_death = BASS_SampleLoad(FALSE,"data\\death.wav",0,0,3,BASS_SAMPLE_OVER_POS);
    HSAMPLE sound_kill = BASS_SampleLoad(FALSE,"data\\emdeath.wav",0,0,3,BASS_SAMPLE_OVER_POS);
    HSAMPLE sound_take = BASS_SampleLoad(FALSE,"data\\take.wav",0,0,3,BASS_SAMPLE_OVER_POS);
    HSAMPLE sound_win = BASS_SampleLoad(FALSE,"data\\win.wav",0,0,3,BASS_SAMPLE_OVER_POS);

    HSTREAM music_menu = BASS_StreamCreateFile(FALSE,"data\\swamprun-menu.mp3",0,0,BASS_SAMPLE_LOOP);
    HSTREAM music_game = BASS_StreamCreateFile(FALSE,"data\\swamprun.mp3",0,0,BASS_SAMPLE_LOOP);

    glClearColor(0,0,0,0);
    glViewport(0,0,800,600);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,800,600,0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    unsigned int jump_start_time;
    unsigned int die_start_time;
    

    levelRestart();
    player_sprite = player_spr_idle[0]; 

    int menuButton = 0 ;

    int fail_start_time;
    int great_start_time;
    int win_start_time;

    bool paused = false;

    fadein_start_time = time();

    BASS_ChannelSetAttribute(music_menu,BASS_ATTRIB_VOL,0.5f);
    BASS_ChannelSetAttribute(music_game,BASS_ATTRIB_VOL,0.5f);

    BASS_ChannelPlay(music_menu,TRUE);

    while(1)
    {
        processEvents();

        if (game_state==GS_MENU)
        {
            if (eventQuit || keyDown[KEY_ESC]) { quit(); }
            if (keyDown[KEY_DOWN] && menuButton==0) { menuButton = 1; }
            if (keyDown[KEY_UP] && menuButton==1) { menuButton = 0; }
            
            if (keyDown[KEY_ENTER] && menuButton==0)
            { 
                BASS_ChannelStop(music_menu);
                BASS_ChannelPlay(music_game,TRUE);

                fadein_start_time = time(); 
                game_state = GS_PLAY; 

                lives = 3;

                level = 0;
                levelRestart(); 
            }

            if (keyDown[KEY_ENTER] && menuButton==1) { quit(); }

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            glEnable(GL_TEXTURE_2D);
            
            drawImage(startScreen,0,0);

            if (menuButton==0) drawImage(buttonRun,360,440);
            if (menuButton==1) drawImage(buttonExit,360,506);
            
            glDisable(GL_TEXTURE_2D);

            fadein(0,0,0,800);
        }
        else if (game_state==GS_PLAY)
        {
            if (eventQuit || keyDown[KEY_ESC])
            {
                BASS_ChannelStop(music_game);
                BASS_ChannelPlay(music_menu,TRUE);
                fadein_start_time = time();
                game_state = GS_MENU;
                continue;
            }

            /*
            // !!! EDIT MODE

            if (keyDown[KEY_R]) { paused=true; levelRestart(); }
            if (keyDown[KEY_1]) { paused=true; level=0; levelRestart(); }
            if (keyDown[KEY_2]) { paused=true; level=1; levelRestart(); }
            if (keyDown[KEY_3]) { paused=true; level=2; levelRestart(); }
            if (keyDown[KEY_4]) { paused=true; level=3; levelRestart(); }
            if (keyDown[KEY_5]) { paused=true; level=4; levelRestart(); }
            if (keyDown[KEY_6]) { paused=true; level=5; levelRestart(); }
            if (keyDown[KEY_7]) { paused=true; level=6; levelRestart(); }
            if (keyDown[KEY_8]) { paused=true; level=7; levelRestart(); }
            
            if (keyDown[KEY_ENTER]) { paused=false; levelRestart(); }

            // !!!

            int level_time = time()-level_start_time;
             
            if (paused)
            {
                sink_cycle = 0;
            }    
            else
            {
                sink_cycle = level_time / sink_cycle_duration;
            }
            */

            int level_time = time()-level_start_time;
            sink_cycle = level_time / sink_cycle_duration;

            for(int x=0;x<MW;x++)
            for(int y=0;y<MH;y++)
            {
                if ((platformMap(x,y)==P_ISLAND || platformMap(x,y)==P_DINO) && sinkStage(x,y)==5)
                {
                    platformMap(x,y) = P_NONE;
                }
                
                if (entityMap(x,y)!=E_NONE && sinkStage(x,y)==5)
                {
                    entityMap(x,y) = E_NONE;
                }
            }

            if (player_state == PS_IDLING)
            {          
                player_sprite = player_spr_idle[time()%600 < 300]; 
                 
                if (platformMap(player_map_x,player_map_y) == P_NONE)
                { 
                    player_state = PS_DYING;
                    die_start_time = time();
                    playSound(sound_death);
                }

                if (platformMap(player_map_x,player_map_y) == P_MOVER)
                {
                    player_mover = moverMap(player_map_x,player_map_y)-1; //!!!! XXX

                    if (movers[player_mover].state==0 && player_map_x == movers[player_mover].map_x1)
                    {
                        player_state = PS_MOVING;
                    }
                    else
                    {
                        player_state = PS_DYING;
                        die_start_time = time();
                        playSound(sound_death);                     
                    }
                }
                
                if (entityMap(player_map_x,player_map_y) == E_GOAL)
                {
                    player_state = PS_WINNING;
                    win_start_time = time();
                    playSound(sound_win);
                    continue;
                }   

                if (entityMap(player_map_x,player_map_y) == E_ITEM)
                {
                    entityMap(player_map_x,player_map_y) = E_NONE;
                    player_numitems++;
                    playSound(sound_take);
                }   

                if (entityMap(player_map_x,player_map_y) == E_ENEMY)
                {                
                    if (player_numitems>0)
                    {
                        player_numitems--;
                        playSound(sound_kill);
                        entityMap(player_map_x,player_map_y) = E_ENEMY_NEUTRAL;                    
                    }
                    else
                    {
                        if      (player_map_x > 0    && platformMap(player_map_x-1,player_map_y)==P_NONE) { player_state = PS_JUMPING; jump_dir = DIR_LEFT;  jump_start_time = time(); playSound(sound_jump);}
                        else if (player_map_x < MW-1 && platformMap(player_map_x+1,player_map_y)==P_NONE) { player_state = PS_JUMPING; jump_dir = DIR_RIGHT; jump_start_time = time(); playSound(sound_jump);}
                        else if (player_map_y < MH-1 && platformMap(player_map_x,player_map_y+1)==P_NONE) { player_state = PS_JUMPING; jump_dir = DIR_DOWN;  jump_start_time = time(); playSound(sound_jump);}                
                        else if (player_map_y > 0    && platformMap(player_map_x,player_map_y-1)==P_NONE) { player_state = PS_JUMPING; jump_dir = DIR_UP;    jump_start_time = time(); playSound(sound_jump);}
                    }                    
                }
                         
                if      (keyDown[KEY_LEFT]  && player_map_x > 0 )   { player_state = PS_JUMPING; jump_dir = DIR_LEFT;  jump_start_time = time(); playSound(sound_jump); }
                else if (keyDown[KEY_RIGHT] && player_map_x < MW-1) { player_state = PS_JUMPING; jump_dir = DIR_RIGHT; jump_start_time = time(); playSound(sound_jump); }
                else if (keyDown[KEY_UP]    && player_map_y > 0)    { player_state = PS_JUMPING; jump_dir = DIR_UP;    jump_start_time = time(); playSound(sound_jump); }
                else if (keyDown[KEY_DOWN]  && player_map_y < MH-1) { player_state = PS_JUMPING; jump_dir = DIR_DOWN;  jump_start_time = time(); playSound(sound_jump); }
            }
            else if (player_state == PS_JUMPING)
            {
                //player_sprite = player_spr_jump[time()%100 < 50]; 
                player_sprite = player_spr_idle[time()%600 < 300]; 
                
                const int jump_duration = 180;
                unsigned int jump_time = time()-jump_start_time;
                
                if (jump_time>jump_duration)
                {                
                    player_state = PS_IDLING;

                    if      (jump_dir == DIR_LEFT) { player_map_x--; }
                    else if (jump_dir == DIR_RIGHT){ player_map_x++; }
                    else if (jump_dir == DIR_UP)   { player_map_y--; }
                    else if (jump_dir == DIR_DOWN) { player_map_y++; }
                    
                    player_x = player_map_x*TW+player_x_ofs;
                    player_y = player_map_y*TH+player_y_ofs;
                }
                else
                {
                    float jump_time_01 = smoothstep(float(jump_time)/float(jump_duration));
                    //float jump_time_01 = float(jump_time)/float(jump_duration);

                    if      (jump_dir == DIR_LEFT) { player_x = float(player_map_x*TW)-jump_time_01*float(TW)+player_x_ofs; player_y = float(player_map_y*TH+player_y_ofs)-sin(jump_time_01*3.1415f)*float(TW)*0.2+jump_time_01*(float(sinkOffset[sinkStage(player_map_x-1,player_map_y)]-sinkOffset[sinkStage(player_map_x,player_map_y)])); }
                    else if (jump_dir == DIR_RIGHT){ player_x = float(player_map_x*TW)+jump_time_01*float(TW)+player_x_ofs; player_y = float(player_map_y*TH+player_y_ofs)-sin(jump_time_01*3.1415f)*float(TW)*0.2+jump_time_01*(float(sinkOffset[sinkStage(player_map_x+1,player_map_y)]-sinkOffset[sinkStage(player_map_x,player_map_y)])); }
                    else if (jump_dir == DIR_UP)   { player_y = float(player_map_y*TH+player_y_ofs)-sin(jump_time_01*3.1415*0.65)*(1.0f/0.891f)*float(TH)+jump_time_01*(float(sinkOffset[sinkStage(player_map_x,player_map_y-1)]-sinkOffset[sinkStage(player_map_x,player_map_y)]));  }
                    else if (jump_dir == DIR_DOWN) { player_y = float(player_map_y*TH+player_y_ofs)+jump_time_01*float(TH)+jump_time_01*(float(sinkOffset[sinkStage(player_map_x,player_map_y+1)]-sinkOffset[sinkStage(player_map_x,player_map_y)])); }
                }
            }
            else if (player_state == PS_MOVING)
            {
                if (movers[player_mover].state == 0)
                {
                    player_map_x = movers[player_mover].map_x1;
                    
                    player_sprite = player_spr_idle[time()%600 < 300]; 
                    
                    if      (keyDown[KEY_LEFT]  && player_map_x > 0 )   { player_state = PS_JUMPING; jump_dir = DIR_LEFT;  jump_start_time = time(); playSound(sound_jump); }
                    else if (keyDown[KEY_RIGHT] && player_map_x < MW-1) { player_state = PS_JUMPING; jump_dir = DIR_RIGHT; jump_start_time = time(); playSound(sound_jump); }
                    else if (keyDown[KEY_UP]    && player_map_y > 0)    { player_state = PS_JUMPING; jump_dir = DIR_UP;    jump_start_time = time(); playSound(sound_jump); }
                    else if (keyDown[KEY_DOWN]  && player_map_y < MH-1) { player_state = PS_JUMPING; jump_dir = DIR_DOWN;  jump_start_time = time(); playSound(sound_jump); }                
                }
                else
                {
                    player_x = movers[player_mover].x + player_x_ofs;

                    player_sprite = player_spr_moving[time()%1200 < 600]; 
                }
            }
            else if (player_state == PS_DYING)
            {
                const int die_duration = 2000;
                unsigned int die_time = time()-die_start_time;
                
                if (die_time>die_duration)
                {              
                    lives--;

                    if (lives > 0)
                    {
                        fadein_start_time = time();                       
                        levelRestart();                     
                        continue;
                    }
                    else
                    {
                        game_state = GS_FAIL;                        
                        fadein_start_time = time();
                        fail_start_time = time();                     
                        continue;
                    }
                }
                else
                {
                    player_sprite = player_spr_death[die_time > die_duration/5];
                }
            }
            else if (player_state == PS_WINNING)
            {
                player_sprite = player_spr_idle[time()%400 < 200]; 
                player_y = player_map_y*TH+player_y_ofs - abs(sin( float(time()-win_start_time)/1500.0f*4.0f*3.1415f ))*32.0f;

                if (time()-win_start_time > 1500)
                {
                    fadein_start_time = time();
                    great_start_time = time();
                    game_state = GS_GREAT;
                }
            }

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glEnable(GL_TEXTURE_2D);
            
            drawBackground(bkg[(time()/500)%4]);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

            for(int x=0;x<MW;x++) 
            {
                for(int y=0;y<MH;y++)
                {
                    const int platformType = platformMap(x,y);
                    const int entityType = entityMap(x,y);

                    //if (platformType!=P_NONE && platformType!=P_MOVER) drawSprite(platformImage[platformType],x,y,0,0);
                    
                    const int sink_stage = sinkStage(x,y);

                    if (platformType==P_ISLAND && sink_stage != 5) drawSprite(islandImages[sink_stage],x,y,0,0); 
                    if (platformType==P_DINO   && sink_stage != 5) drawSprite(dinoImages[sink_stage],x,y,0,0); 
                    if (entityType!=E_NONE && sinkStage(x,y)!=5) drawSprite(entityImage[entityType],x,y,0,entityOffset[entityType]+sinkOffset[sinkStage(x,y)]);                
                }
            }

         
            for(int i=0;i<movers.size();i++) 
            {
                movers[i].update();
                drawImage(moverImage,movers[i].x,movers[i].y);
            }


            drawImage(player_sprite,player_x,player_y+sinkOffset[sinkStage(player_map_x,player_map_y)]);  

            //drawSprite(player_sprite,player_x,player_y,0,-TH/2);

            glDisable(GL_BLEND);

            glDisable(GL_TEXTURE_2D);
            
            fadein(0,0,0,600);
        }
        else if (game_state == GS_FAIL)
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            glEnable(GL_TEXTURE_2D);
            
            drawImage(failScreen,0,0);
            
            glDisable(GL_TEXTURE_2D);

            fadein(1,1,1,150);

            const int fail_time = time()-fail_start_time;
            const int fail_duration = 4000;
            const int fail_fadeout_duration = 300;

            if (fail_time > (fail_duration-fail_fadeout_duration))
            {                
                float alpha = (fail_time-(fail_duration-fail_fadeout_duration))/float(fail_fadeout_duration);
                blendquad(0,0,0,alpha);
            }
            if (fail_time > fail_duration)
            {
                BASS_ChannelStop(music_game);
                BASS_ChannelPlay(music_menu,TRUE);
                fadein_start_time = time();
                game_state = GS_MENU;
                continue;
            }
        }
        else if (game_state == GS_GREAT)
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            glEnable(GL_TEXTURE_2D);
            
            drawImage(greatScreen,0,0);
            
            glDisable(GL_TEXTURE_2D);

            fadein(1,1,1,150);

            const int great_time = time()-great_start_time;
            const int great_duration = 2100;
            const int great_fadeout_duration = 200;

            if (great_time > (great_duration-great_fadeout_duration))
            {                
                float alpha = (great_time-(great_duration-great_fadeout_duration))/float(great_fadeout_duration);
                blendquad(0,0,0,alpha);
            }
            if (great_time > great_duration)
            {
                fadein_start_time = time();
                game_state = GS_PLAY;

                lives = 3;

                level++;
                level = level % numlevels;
                
                levelRestart();

                continue;
            }
        }       
        SDL_GL_SwapBuffers( );
    }
 
    SDL_Quit();

    return 0;
}