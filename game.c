#include <stdio.h>
#include <string.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"
#include <time.h>
#include <math.h>

typedef struct{
    int x, y, w, h;
    char life;
    int tics;
} Man;

typedef struct{
    double x, y, w, h;
    int tics;
    char life;
} Fighter;

typedef struct{
    double x, y, w, h;
    int tics;
    char life;
    int movex, movey;
    int dt;
} Bomber;

typedef struct{
    double x, y, w, h;
    int tics;
    char life;
} Kam;

typedef struct{
    int x,y,tics, w, h, expolsion;
    char life;
} Explosion;

typedef struct{
    int x,y,r,tics, w, h;
    char life;
} Shots;

typedef struct{
    int x,y,r,tics, w, h;
    char life;
} Mine;

typedef struct{
    double r, x,y;
    int n;
    int tics;
    char life;
} EnShots;

typedef struct{
    double x,y,w;
    int tics;
    char life;
    int n;
} Big_Astroid;

typedef struct{
    double x,y,w;
    int tics;
    char life;
    int n;
} Small_Astroid;

typedef struct {
    Man man;
    Big_Astroid big_astroid[20];
    Small_Astroid small_astroid[80];
    SDL_Texture *bigass_im;
    SDL_Texture *smallass1_im;
    SDL_Texture *smallass2_im;
    SDL_Texture *smallass3_im;
    SDL_Texture *smallass4_im;
    Kam kam[40];
    EnShots enshots[1000];
    Explosion explosion[100];
    SDL_Texture *img;
    Shots shots[1000];
    Fighter fighter[40];
    Bomber bomber[40];
    Mine mine[50];
    TTF_Font *ttf;
    SDL_Texture *mineim;
    SDL_Texture *bomberim;
    SDL_Texture *shotim;
    SDL_Texture *fightim;
    SDL_Texture *kamim;
    SDL_Texture *b1;
    SDL_Texture *b2;
    SDL_Texture *b3;
    SDL_Texture *b4;
    SDL_Texture *b5;
    SDL_Texture *b6;
    SDL_Texture *b7;
    SDL_Texture *b8;
    SDL_Texture *b9;
    SDL_Texture *b10;
    SDL_Texture *b11;
    Mix_Chunk *laser;
    Mix_Chunk *enlaser;
    int max_bigass;
    int max_smallass;
    int minenum;
    int shotsnum;
    int shotem;
    int fightnum;
    int assnum;
    int smallassnum;
    int asstic;
    int kamnum;
    int bombernum;
    int fighttic;
    int kamtic;
    int bombertic;
    int height;
    int width;
    int maxfighter;
    int maxkam;
    int maxshots;
    int maxenshots;
    int maxmines;
    int maxbombers;
    float rot;
    int explosionnum;
    int maxexplosion;
    int tics;
} GameState;

int processEvents(SDL_Window *window, GameState *game) {
    SDL_Event event;
    int done = 0;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_WINDOWEVENT_CLOSE: {
                if (window) {
                    SDL_DestroyWindow(window);
                    window = NULL;
                    done = 1;
                }
            }
                break;
            case SDL_KEYDOWN: {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        done = 1;
                        break;
                        }
                }
                break;
                case SDL_QUIT:
                    done = 1;
                break;
            }
        }
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_A]) {
            game->man.x -= 10;
            if (game->man.x < 0){
                game->man.x = game->width-5;
            } else if (game->man.x > game->width){
                game->man.x = 5;
            }
            game->rot = -90.0f;
        }
        if (state[SDL_SCANCODE_D]) {
            game->man.x += 10;
            game->rot = 90.0f;
            if (game->man.x < 0){
                game->man.x = game->width-5;
            } else if (game->man.x > game->width){
                game->man.x = 5;
            }
        }
        if (state[SDL_SCANCODE_W]) {
            game->man.y -= 10;
            game->rot = 00.0f;
            if (game->man.y < 0){
                game->man.y = game->height-5;
            } else if (game->man.y > game->height){
                game->man.y = 5;
            }
        }
        if (state[SDL_SCANCODE_S]) {
            game->man.y += 10;
            game->rot = 180.0f;
            if (game->man.y < 0){
                game->man.y = game->height-5;
            } else if (game->man.y > game->height){
                game->man.y = 5;
            }
        }
        if (state[SDL_SCANCODE_S] & state[SDL_SCANCODE_A]) {
            game->rot = 225.0f;
        }
        if (state[SDL_SCANCODE_S] & state[SDL_SCANCODE_D]) {
            game->rot = 135.0f;
        }
        if (state[SDL_SCANCODE_W] & state[SDL_SCANCODE_A]) {
            game->rot = -45.0f;
        }if (state[SDL_SCANCODE_W] & state[SDL_SCANCODE_D]) {
            game->rot = 45.0f;
        }if (state[SDL_SCANCODE_SPACE]) {
            int dt = SDL_GetTicks()-game->shots->tics;
            if (dt >= 20){
                if (game->rot == 0.00f) {
                    Mix_PlayChannel(2, game->laser, 0);
                    Mix_Volume(2, 10);
                    game->shots[game->shotsnum].x = game->man.x + 18;
                    game->shots[game->shotsnum].y = game->man.y;
                    game->shots[game->shotsnum].r = 0;
                    game->shots[game->shotsnum].life = 'y';
                    game->shotsnum += 1;
                    if (game->shotsnum == game->maxshots) {
                        game->shotsnum = 0;
                    }
                } else if (game->rot == 90.00f) {
                    Mix_PlayChannel(2, game->laser, 0);
                    Mix_Volume(2, 10);
                    game->shots[game->shotsnum].x = game->man.x + 36;
                    game->shots[game->shotsnum].y = game->man.y + 20;
                    game->shots[game->shotsnum].r = 2;
                    game->shots[game->shotsnum].life = 'y';
                    game->shotsnum += 1;
                    if (game->shotsnum == game->maxshots) {
                        game->shotsnum = 0;
                    }
                } else if (game->rot == -90.00f) {
                    Mix_PlayChannel(2, game->laser, 0);
                    Mix_Volume(2, 10);
                    game->shots[game->shotsnum].x = game->man.x;
                    game->shots[game->shotsnum].y = game->man.y + 20;
                    game->shots[game->shotsnum].r = 6;
                    game->shots[game->shotsnum].life = 'y';
                    game->shotsnum += 1;
                    if (game->shotsnum == game->maxshots) {
                        game->shotsnum = 0;
                    }
                }else if (game->rot == 180.00f) {
                    Mix_PlayChannel(2, game->laser, 0);
                    Mix_Volume(2, 10);
                    game->shots[game->shotsnum].x = game->man.x + 20;
                    game->shots[game->shotsnum].y = game->man.y + 36;
                    game->shots[game->shotsnum].r = 4;
                    game->shots[game->shotsnum].life = 'y';
                    game->shotsnum += 1;
                    if (game->shotsnum == game->maxshots) {
                        game->shotsnum = 0;
                    }
                }else if (game->rot == 225.00f) {
                    Mix_PlayChannel(2, game->laser, 0);
                    Mix_Volume(2, 10);
                    game->shots[game->shotsnum].x = game->man.x+8;
                    game->shots[game->shotsnum].y = game->man.y + 34;
                    game->shots[game->shotsnum].r = 5;
                    game->shots[game->shotsnum].life = 'y';
                    game->shotsnum += 1;
                    if (game->shotsnum == game->maxshots) {
                        game->shotsnum = 0;
                    }
                }else if (game->rot == 135.00f) {
                    Mix_PlayChannel(2, game->laser, 0);
                    Mix_Volume(2, 10);
                    game->shots[game->shotsnum].x = game->man.x + 36;
                    game->shots[game->shotsnum].y = game->man.y + 32;
                    game->shots[game->shotsnum].r = 3;
                    game->shots[game->shotsnum].life = 'y';
                    game->shotsnum += 1;
                    if (game->shotsnum == game->maxshots) {
                        game->shotsnum = 0;
                    }
                }else if (game->rot == 45.00f) {
                    Mix_PlayChannel(2, game->laser, 0);
                    Mix_Volume(2, 10);
                    game->shots[game->shotsnum].x = game->man.x + 34;
                    game->shots[game->shotsnum].y = game->man.y + 6;
                    game->shots[game->shotsnum].r = 1;
                    game->shots[game->shotsnum].life = 'y';
                    game->shotsnum += 1;
                    if (game->shotsnum == game->maxshots) {
                        game->shotsnum = 0;
                    }
                }
                else if (game->rot == -45.00f) {
                    Mix_PlayChannel(2, game->laser, 0);
                    Mix_Volume(2, 10);
                    game->shots[game->shotsnum].x = game->man.x + 6;
                    game->shots[game->shotsnum].y = game->man.y + 4;
                    game->shots[game->shotsnum].r = 7;
                    game->shots[game->shotsnum].life = 'y';
                    game->shotsnum += 1;
                    if (game->shotsnum == game->maxshots) {
                        game->shotsnum = 0;
                    }
                }
            }
            game->shots->tics = SDL_GetTicks();

        }


        return done;
    }

void moveEnemy(GameState *game){
    int dtt = SDL_GetTicks()-game->asstic;
    double dis = 0;
    double endis = 0;
    double angle = 0;

    if (dtt > 8000){
        game->big_astroid[game->assnum].x = random()%game->width;
        game->big_astroid[game->assnum].y = -random()%100;
        game->big_astroid[game->assnum].life = 'y';
        if (game->big_astroid[game->assnum].x <= game->width/2){
            game->big_astroid[game->assnum].n = 1;
        } else{
            game->big_astroid[game->assnum].n = 2;
        }
        game->assnum += 1;
        if (game->assnum == game->max_bigass-2){
            game->assnum = 0;
        }
        game->asstic = SDL_GetTicks();
    }

    for (int i = 0; i<game->max_bigass;i++){
        if (game->big_astroid[i].life == 'y'){
            if (game->big_astroid[i].n == 1){
                game->big_astroid[i].x += 3;
                game->big_astroid[i].y += 3;
            } else{
                game->big_astroid[i].x -= 3;
                game->big_astroid[i].y += 3;
            }
        }
    }

    for (int i = 0; i<game->max_smallass; i++){
        if (game->small_astroid[i].life == 'y'){
            if (game->small_astroid[i].n == 1){
                game->small_astroid[i].x += 2;
                game->small_astroid[i].y += 2;
            } else if (game->small_astroid[i].n == 2){
                game->small_astroid[i].x -= 2;
                game->small_astroid[i].y += 2;
            }else if (game->small_astroid[i].n == 3){
                game->small_astroid[i].x -= 2;
                game->small_astroid[i].y -= 2;
            }else if (game->small_astroid[i].n == 4){
                game->small_astroid[i].x += 2;
                game->small_astroid[i].y -= 2;
            }
        }
    }


    for (int i = 0; i<game->maxbombers;i++){
        int dt = SDL_GetTicks()-game->bomber[i].tics;
        if (game->bomber[i].life == 'y'){
            if (game->bomber[i].movex == 0){
                game->bomber[i].x += 5;
                game->bomber[i].y;
            } else if (game->bomber[i].movex == 1){
                game->bomber[i].x -= 5;
                game->bomber[i].y;
            }if (game->bomber[i].movey == 0){
                game->bomber[i].x;
                game->bomber[i].y += 5;
            } else if(game->bomber[i].movey == 1){
                game->bomber[i].x;
                game->bomber[i].y -= 5;
            }
            if (game->bomber[i].x > game->width){
                game->bomber[i].movex = 1;
            } else if (game->bomber[i].x < 0){
                game->bomber[i].movex = 0;
            }
            if (game->bomber[i].y > game->height){
                game->bomber[i].movey = 1;
            } else if (game->bomber[i].y < 0) {
                game->bomber[i].movey = 0;
            }
            if (dt >= game->bomber[i].dt && game->bomber[i].x < game->width && game->bomber[i].x > 0 && game->bomber[i].y < game->height && game->bomber[i].y > 0){
                game->mine[game->minenum].x = game->bomber[i].x+14;
                game->mine[game->minenum].y = game->bomber[i].y+14;
                game->mine[game->minenum].life = 'y';
                if (game->minenum < game->maxmines-1) {
                    game->minenum += 1;
                    if (game->minenum == game->maxmines-2){
                        game->minenum = 0;
                    }
                    game->bomber[i].tics = SDL_GetTicks();
                }
            }
        }

    }
    for (int i = 0; i<game->maxkam; i++){
        if (game->kam->life == 'y'){
            angle = atan((game->kam[i].y - game->man.y) / (game->kam[i].x - game->man.x));
            if (game->man.x < game->kam[i].x) {
                game->kam[i].y += 8 * sin(angle) * -1;
                game->kam[i].x += 8 * cos(angle) * -1;
            } else if (game->man.x > game->kam[i].x) {
                game->kam[i].y += 8 * sin(angle);
                game->kam[i].x += 8 * cos(angle);
            }
            dis = sqrt((pow((game->man.x+14.5 - game->kam[i].x), 2.0)) + pow((game->man.y+13.5 - game->kam[i].y), 2.0));
        }
            for (int j = 0; j < game->maxkam; j++) {
                endis = sqrt((pow((game->kam[j].x - game->kam[i].x), 2.0)) +
                             pow((game->kam[j].y - game->kam[i].y), 2.0));
                if (0 <= endis && endis < 50) {
                    game->kam[i].x -= (game->kam[j].x - game->kam[i].x) * .02;
                    game->kam[i].y -= (game->kam[j].y - game->kam[i].y) * .02;
                }
            }
        if (game->fighter[i].life == 'y') {
            int dt = SDL_GetTicks()-game->enshots[i].tics;
            if (dt >= 700  && game->fighter[i].x > 0) {
                angle = atan((game->fighter[i].y - game->man.y) / (game->fighter[i].x - game->man.x));
                if (game->man.x < game->fighter[i].x) {
                    game->enshots[game->shotem].n = -1;
                } else if (game->man.x > game->fighter[i].x) {
                    game->enshots[game->shotem].n = 1;
                }
                Mix_PlayChannel(3, game->enlaser, 0);
                Mix_Volume(3, 10);
                game->enshots[game->shotem].r = angle;
                game->enshots[game->shotem].x = game->fighter[i].x+14.5;
                game->enshots[game->shotem].y = game->fighter[i].y+14.5;
                game->enshots[game->shotem].life = 'y';
                game->shotem += 1;
                if (game->shotem == game->maxshots-1){
                    game->shotem = 0;
                }
                game->enshots[i].tics = SDL_GetTicks();

            }
            dis = sqrt((pow((game->man.x - game->fighter[i].x), 2.0)) + pow((game->man.y - game->fighter[i].y), 2.0));
            if (dis > 0) {
                game->fighter[i].x += (game->man.x - game->fighter[i].x) * .009;
                game->fighter[i].y += (game->man.y - game->fighter[i].y) * .009;
            }
            for (int j = 0; j < game->maxfighter; j++) {
                endis = sqrt((pow((game->fighter[j].x - game->fighter[i].x), 2.0)) +
                             pow((game->fighter[j].y - game->fighter[i].y), 2.0));
                if (0 <= endis && endis < 50) {
                    game->fighter[i].x -= (game->fighter[j].x - game->fighter[i].x) * .02;
                    game->fighter[i].y -= (game->fighter[j].y - game->fighter[i].y) * .02;
                }
            }
        }
    }

}


void collison(GameState *game, Mix_Chunk *ex, Mix_Chunk *aex) {
    float iw = game->man.w;
    float ih = game->man.h;
    float iy = game->man.y;
    float ix = game->man.x;
    for (int j = 0; j < game->maxmines; j++) {
        float my = game->mine[j].y;
        float mx = game->mine[j].x;
        float mw = game->mine->w;
        float mh = game->mine->h;
        if (!(mx + mw < ix || mx > ix + iw || my > iy + ih || my + mh < iy) && game->mine[j].life == 'y' &&
            game->man.life == 'y') {
            game->man.life = 'e';
            game->mine[j].life = 'e';
            Mix_PlayChannel(-1, ex, 0);
            game->explosion[game->explosionnum].x = game->man.x - 10;
            game->explosion[game->explosionnum].y = game->man.y - 60;
            game->explosion[game->explosionnum].tics = SDL_GetTicks();
            game->explosion[game->explosionnum].life = 'y';
            game->explosion[game->explosionnum].expolsion = 0;
            game->explosionnum += 1;
            game->explosion[game->explosionnum].expolsion = 0;
            game->explosion[game->explosionnum].x = game->mine[j].x;
            game->explosion[game->explosionnum].y = game->mine[j].y - 60;
            game->explosion[game->explosionnum].tics = SDL_GetTicks();
            game->explosion[game->explosionnum].life = 'y';
            game->explosionnum += 1;

            for (int i = 0; i < game->max_bigass; i++) {
                float jy = game->big_astroid[i].y;
                float jx = game->big_astroid[i].x;
                float jw = game->big_astroid->w;
                if (!(mx + 87.5 < jx || mx - 62.5 > jx + jw || my - 62.5 > jy + jw || my + 82.5 < jy) &&
                    game->mine[j].life == 'e' && game->big_astroid[i].life == 'y') {
                    game->big_astroid[i].life = 'e';
                    Mix_PlayChannel(-1, aex, 0);
                    game->small_astroid[game->smallassnum].x = game->big_astroid[i].x + 40;
                    game->small_astroid[game->smallassnum].y = game->big_astroid[i].y + 40;
                    game->small_astroid[game->smallassnum].n = 1;
                    game->small_astroid[game->smallassnum].life = 'y';
                    game->smallassnum += 1;
                    game->small_astroid[game->smallassnum].x = game->big_astroid[i].x + 40;
                    game->small_astroid[game->smallassnum].y = game->big_astroid[i].y + 40;
                    game->small_astroid[game->smallassnum].n = 2;
                    game->small_astroid[game->smallassnum].life = 'y';
                    game->smallassnum += 1;
                    game->small_astroid[game->smallassnum].x = game->big_astroid[i].x + 40;
                    game->small_astroid[game->smallassnum].y = game->big_astroid[i].y + 40;
                    game->small_astroid[game->smallassnum].n = 3;
                    game->small_astroid[game->smallassnum].life = 'y';
                    game->smallassnum += 1;
                    game->small_astroid[game->smallassnum].x = game->big_astroid[i].x + 40;
                    game->small_astroid[game->smallassnum].y = game->big_astroid[i].y + 40;
                    game->small_astroid[game->smallassnum].n = 4;
                    game->small_astroid[game->smallassnum].life = 'y';
                    game->smallassnum += 1;
                    if (game->smallassnum == game->max_smallass - 10){
                        game->smallassnum = 0;
                    }
                }
            }
            for (int i = 0; i < game->max_smallass; i++) {
                float jy = game->small_astroid[i].y;
                float jx = game->small_astroid[i].x;
                float jw = game->small_astroid->w;
                if (!(mx + 87.5 < jx || mx - 62.5 > jx + jw || my - 62.5 > jy + jw || my + 82.5 < jy) &&
                    game->mine[j].life == 'e' && game->small_astroid[i].life == 'y') {
                    game->small_astroid[i].life = 'e';
                    Mix_PlayChannel(-1, aex, 0);
                }
            }
            for (int i = 0; i < game->maxfighter; i++) {
                float jy = game->fighter[i].y;
                float jx = game->fighter[i].x;
                float wy = game->kam[i].y;
                float wx = game->kam[i].x;
                float by = game->bomber[i].y;
                float bx = game->bomber[i].x;
                if (!(mx + 87.5 < jx || mx - 62.5 > jx + iw || my - 62.5 > jy + ih || my + 82.5 < jy) &&
                    game->mine[j].life == 'e' && game->fighter[i].life == 'y') {
                    game->fighter[i].life = 'e';
                    game->mine[j].life = 'e';
                    Mix_PlayChannel(-1, ex, 0);
                    game->explosion[game->explosionnum].x = game->fighter[i].x - 90;
                    game->explosion[game->explosionnum].y = game->fighter[i].y - 60;
                    game->explosion[game->explosionnum].expolsion = 0;
                    game->explosion[game->explosionnum].tics = SDL_GetTicks();
                    game->explosion[game->explosionnum].life = 'y';
                    game->explosionnum += 1;
                    game->explosion[game->explosionnum].expolsion = 0;
                    game->explosion[game->explosionnum].x = game->mine[j].x;
                    game->explosion[game->explosionnum].y = game->mine[j].y - 60;
                    game->explosion[game->explosionnum].tics = SDL_GetTicks();
                    game->explosion[game->explosionnum].life = 'y';
                    game->explosionnum += 1;
                }
                if (!(mx + 87.5 < wx || mx - 62.5 > wx + iw || my - 62.5 > wy + ih || my + 82.5 < wy) &&
                    game->mine[j].life == 'e' && game->kam[i].life == 'y') {
                    game->kam[i].life = 'e';
                    game->mine[j].life = 'e';
                    Mix_PlayChannel(-1, ex, 0);
                    game->explosion[game->explosionnum].x = game->kam[i].x - 90;
                    game->explosion[game->explosionnum].y = game->kam[i].y - 60;
                    game->explosion[game->explosionnum].expolsion = 0;
                    game->explosion[game->explosionnum].tics = SDL_GetTicks();
                    game->explosion[game->explosionnum].life = 'y';
                    game->explosionnum += 1;
                }
                if (!(mx + 87.5 < bx || mx - 62.5 > bx + iw || my - 62.5 > by + ih || my + 82.5 < by) &&
                    game->mine[j].life == 'e' && game->bomber[i].life == 'y') {
                    game->bomber[i].life = 'e';
                    game->mine[j].life = 'e';
                    Mix_PlayChannel(-1, ex, 0);
                    game->explosion[game->explosionnum].x = game->bomber[i].x - 90;
                    game->explosion[game->explosionnum].y = game->bomber[i].y - 60;
                    game->explosion[game->explosionnum].expolsion = 0;
                    game->explosion[game->explosionnum].tics = SDL_GetTicks();
                    game->explosion[game->explosionnum].life = 'y';
                    game->explosionnum += 1;


                }

            }

        }
        for (int i = 0; i < game->maxshots; i++) {
            if (game->shots[i].life == 'y') {
                float sy = game->shots[i].y;
                float sx = game->shots[i].x;
                if (!(mx + mw < sx || mx > sx + 10 || my > sy + 10 || my + mh < sy) &&
                    game->mine[j].life == 'y') {
                    game->mine[j].life = 'e';
                    game->shots[i].life = 'n';
                    Mix_PlayChannel(-1, ex, 0);
                    game->mine[j].tics = SDL_GetTicks();
                    game->explosion[game->explosionnum].expolsion = 0;
                    game->explosion[game->explosionnum].x = game->mine[j].x;
                    game->explosion[game->explosionnum].y = game->mine[j].y - 60;
                    game->explosion[game->explosionnum].tics = SDL_GetTicks();
                    game->explosion[game->explosionnum].life = 'y';
                    game->explosionnum += 1;
                }
                if (!(mx + 87.5 < ix || mx - 62.5 > ix + iw || my - 62.5 > iy + ih || my + 82.5 < iy) &&
                    game->mine[j].life == 'e' && game->man.life == 'y') {
                    game->man.life = 'e';
                    game->mine[j].life = 'e';
                    Mix_PlayChannel(-1, ex, 0);
                    game->explosion[game->explosionnum].x = game->man.x - 90;
                    game->explosion[game->explosionnum].y = game->man.y - 60;
                    game->explosion[game->explosionnum].tics = SDL_GetTicks();
                    game->explosion[game->explosionnum].life = 'y';
                    game->explosion[game->explosionnum].expolsion = 0;
                    game->explosionnum += 1;
                }
                for (int w = 0; w < game->max_bigass; w++) {
                    float jy = game->big_astroid[w].y;
                    float jx = game->big_astroid[w].x;
                    float jw = game->big_astroid->w;
                    if (!(mx + 87.5 < jx || mx - 62.5 > jx + jw || my - 62.5 > jy + jw || my + 82.5 < jy) &&
                        game->mine[j].life == 'e' && game->big_astroid[w].life == 'y') {
                        game->big_astroid[w].life = 'e';
                        Mix_PlayChannel(-1, aex, 0);
                        game->small_astroid[game->smallassnum].x = game->big_astroid[w].x + 40;
                        game->small_astroid[game->smallassnum].y = game->big_astroid[w].y + 40;
                        game->small_astroid[game->smallassnum].n = 1;
                        game->small_astroid[game->smallassnum].life = 'y';
                        game->smallassnum += 1;
                        game->small_astroid[game->smallassnum].x = game->big_astroid[w].x + 40;
                        game->small_astroid[game->smallassnum].y = game->big_astroid[w].y + 40;
                        game->small_astroid[game->smallassnum].n = 2;
                        game->small_astroid[game->smallassnum].life = 'y';
                        game->smallassnum += 1;
                        game->small_astroid[game->smallassnum].x = game->big_astroid[w].x + 40;
                        game->small_astroid[game->smallassnum].y = game->big_astroid[w].y + 40;
                        game->small_astroid[game->smallassnum].n = 3;
                        game->small_astroid[game->smallassnum].life = 'y';
                        game->smallassnum += 1;
                        game->small_astroid[game->smallassnum].x = game->big_astroid[w].x + 40;
                        game->small_astroid[game->smallassnum].y = game->big_astroid[w].y + 40;
                        game->small_astroid[game->smallassnum].n = 4;
                        game->small_astroid[game->smallassnum].life = 'y';
                        game->smallassnum += 1;
                        if (game->smallassnum == game->max_smallass - 10){
                            game->smallassnum = 0;
                        }
                    }
                }
                for (int w = 0; w < game->max_smallass; w++) {
                    float jy = game->small_astroid[w].y;
                    float jx = game->small_astroid[w].x;
                    float jw = game->small_astroid->w;
                    if (!(mx + 87.5 < jx || mx - 62.5 > jx + jw || my - 62.5 > jy + jw || my + 82.5 < jy) &&
                        game->mine[j].life == 'e' && game->small_astroid[w].life == 'y') {
                        game->small_astroid[w].life = 'e';
                        Mix_PlayChannel(-1, aex, 0);
                    }
                }

                for (int w = 0; w < game->maxfighter; w++) {
                    float jy = game->fighter[w].y;
                    float jx = game->fighter[w].x;
                    float wy = game->kam[w].y;
                    float wx = game->kam[w].x;
                    float by = game->bomber[w].y;
                    float bx = game->bomber[w].x;
                    if (!(mx + 87.5 < jx || mx - 62.5 > jx + iw || my - 62.5 > jy + ih || my + 82.5 < jy) &&
                        game->mine[j].life == 'e' && game->fighter[w].life == 'y') {
                        game->fighter[w].life = 'e';
                        game->mine[j].life = 'e';
                        Mix_PlayChannel(-1, ex, 0);
                        game->explosion[game->explosionnum].x = game->fighter[w].x - 90;
                        game->explosion[game->explosionnum].y = game->fighter[w].y - 60;
                        game->explosion[game->explosionnum].expolsion = 0;
                        game->explosion[game->explosionnum].tics = SDL_GetTicks();
                        game->explosion[game->explosionnum].life = 'y';
                        game->explosionnum += 1;
                    }
                    if (!(mx + 87.5 < wx || mx - 62.5 > wx + iw || my - 62.5 > wy + ih || my + 82.5 < wy) &&
                        game->mine[j].life == 'e' && game->kam[w].life == 'y') {
                        game->kam[w].life = 'e';
                        game->mine[j].life = 'e';
                        Mix_PlayChannel(-1, ex, 0);
                        game->explosion[game->explosionnum].x = game->kam[w].x - 90;
                        game->explosion[game->explosionnum].y = game->kam[w].y - 60;
                        game->explosion[game->explosionnum].expolsion = 0;
                        game->explosion[game->explosionnum].tics = SDL_GetTicks();
                        game->explosion[game->explosionnum].life = 'y';
                        game->explosionnum += 1;
                    }
                    if (!(mx + 87.5 < bx || mx - 62.5 > bx + iw || my - 62.5 > by + ih || my + 82.5 < by) &&
                        game->mine[j].life == 'e' && game->bomber[w].life == 'y') {
                        game->bomber[w].life = 'e';
                        game->mine[j].life = 'e';
                        Mix_PlayChannel(-1, ex, 0);
                        game->explosion[game->explosionnum].x = game->bomber[w].x - 90;
                        game->explosion[game->explosionnum].y = game->bomber[w].y - 60;
                        game->explosion[game->explosionnum].expolsion = 0;
                        game->explosion[game->explosionnum].tics = SDL_GetTicks();
                        game->explosion[game->explosionnum].life = 'y';
                        game->explosionnum += 1;
                    }

                }
            }
        }
    }
    for (int i = 0; i < game->maxenshots; i++) {
        if (game->enshots[i].life == 'y') {
            float sy = game->enshots[i].y;
            float sx = game->enshots[i].x;
            if (!(ix+iw < sx || ix > sx + 10 || iy > sy+10 || iy+ih < sy) &&
                  game->man.life == 'y') {
                  game->man.life = 'e';
                  game->enshots[i].life = 'n';
                  Mix_PlayChannel(-1, ex, 0);
                  game->explosion[game->explosionnum].x = game->man.x - 90;
                  game->explosion[game->explosionnum].y = game->man.y - 60;
                  game->explosion[game->explosionnum].tics = SDL_GetTicks();
                  game->explosion[game->explosionnum].expolsion = 0;
                  game->explosion[game->explosionnum].life = 'y';
                  game->explosionnum += 1;
                }
            }

        }
    for (int j = 0; j < game->max_smallass; j++) {
        float jy = game->small_astroid[j].y;
        float jx = game->small_astroid[j].x;
        float jw = game->small_astroid->w;
        if (!(ix + ih < jx || ix > jx + jw || iy > jy + jw || iy + ih < jy) &&
            game->man.life == 'y' && game->small_astroid[j].life == 'y') {
            game->man.life = 'e';
            Mix_PlayChannel(-1, ex, 0);
            game->explosion[game->explosionnum].x = game->man.x - 90;
            game->explosion[game->explosionnum].y = game->man.y - 60;
            game->explosion[game->explosionnum].tics = SDL_GetTicks();
            game->explosion[game->explosionnum].expolsion = 0;
            game->explosion[game->explosionnum].life = 'y';
            game->explosionnum += 1;
        }
        for (int i = 0; i < game->maxshots; i++) {
            if (game->shots[i].life == 'y') {
                float sy = game->shots[i].y;
                float sx = game->shots[i].x;
                if (!(jx + jw < sx || jx > sx + 10 || jy > sy + 10 || jy + jw < sy) &&
                    game->small_astroid[j].life == 'y') {
                    game->small_astroid[j].life = 'e';
                    game->shots[i].life = 'n';
                    Mix_PlayChannel(-1, aex, 0);
                }
            }
        }
    }
    for (int j = 0; j < game->max_bigass; j++) {
        float jy = game->big_astroid[j].y;
        float jx = game->big_astroid[j].x;
        float jw = game->big_astroid->w;
        if (!(ix + ih < jx || ix > jx + jw || iy > jy + jw || iy + ih < jy) &&
            game->man.life == 'y' && game->big_astroid[j].life == 'y') {
            game->man.life = 'e';
            Mix_PlayChannel(-1, ex, 0);
            game->explosion[game->explosionnum].x = game->man.x - 90;
            game->explosion[game->explosionnum].y = game->man.y - 60;
            game->explosion[game->explosionnum].tics = SDL_GetTicks();
            game->explosion[game->explosionnum].expolsion = 0;
            game->explosion[game->explosionnum].life = 'y';
            game->explosionnum += 1;
        }
        for (int i = 0; i < game->maxshots; i++) {
            if (game->shots[i].life == 'y') {
                float sy = game->shots[i].y;
                float sx = game->shots[i].x;
                if (!(jx + jw < sx || jx > sx + 10 || jy > sy + 10 || jy + jw < sy) &&
                    game->big_astroid[j].life == 'y') {
                    game->big_astroid[j].life = 'e';
                    game->shots[i].life = 'n';
                    Mix_PlayChannel(-1, aex, 0);
                    game->small_astroid[game->smallassnum].x = game->big_astroid[j].x + 40;
                    game->small_astroid[game->smallassnum].y = game->big_astroid[j].y + 40;
                    game->small_astroid[game->smallassnum].n = 1;
                    game->small_astroid[game->smallassnum].life = 'y';
                    game->smallassnum += 1;
                    game->small_astroid[game->smallassnum].x = game->big_astroid[j].x + 40;
                    game->small_astroid[game->smallassnum].y = game->big_astroid[j].y + 40;
                    game->small_astroid[game->smallassnum].n = 2;
                    game->small_astroid[game->smallassnum].life = 'y';
                    game->smallassnum += 1;
                    game->small_astroid[game->smallassnum].x = game->big_astroid[j].x + 40;
                    game->small_astroid[game->smallassnum].y = game->big_astroid[j].y + 40;
                    game->small_astroid[game->smallassnum].n = 3;
                    game->small_astroid[game->smallassnum].life = 'y';
                    game->smallassnum += 1;
                    game->small_astroid[game->smallassnum].x = game->big_astroid[j].x + 40;
                    game->small_astroid[game->smallassnum].y = game->big_astroid[j].y + 40;
                    game->small_astroid[game->smallassnum].n = 4;
                    game->small_astroid[game->smallassnum].life = 'y';
                    game->smallassnum += 1;
                    if (game->smallassnum == game->max_smallass - 10){
                        game->smallassnum = 0;
                    }
                }
            }
        }
    }
        for (int j = 0; j < game->maxfighter; j++) {
            float jy = game->fighter[j].y;
            float jx = game->fighter[j].x;
            float wy = game->kam[j].y;
            float wx = game->kam[j].x;
            float by = game->bomber[j].y;
            float bx = game->bomber[j].x;
            if (!(ix+ih < jx || ix > jx + iw || iy > jy + ih || iy + ih < jy) &&
                game->man.life == 'y' && game->fighter[j].life == 'y') {
                game->man.life = 'e';
                Mix_PlayChannel(-1, ex, 0);
                game->explosion[game->explosionnum].x = game->man.x - 90;
                game->explosion[game->explosionnum].y = game->man.y - 60;
                game->explosion[game->explosionnum].tics = SDL_GetTicks();
                game->explosion[game->explosionnum].expolsion = 0;
                game->explosion[game->explosionnum].life = 'y';
                game->explosionnum += 1;
            }
            if (!(ix+ih < wx || ix > wx + iw || iy > wy + ih || iy + ih < wy) &&
                game->man.life == 'y' && game->kam[j].life == 'y') {
                game->man.life = 'e';
                Mix_PlayChannel(-1, ex, 0);
                game->explosion[game->explosionnum].x = game->man.x - 90;
                game->explosion[game->explosionnum].y = game->man.y - 60;
                game->explosion[game->explosionnum].tics = SDL_GetTicks();
                game->explosion[game->explosionnum].expolsion = 0;
                game->explosion[game->explosionnum].life = 'y';
                game->explosionnum += 1;
            }
            if (!(ix+ih < bx || ix > bx + iw || iy > by + ih || iy + ih < by) &&
                game->man.life == 'y' && game->bomber[j].life == 'y') {
                game->man.life = 'e';
                Mix_PlayChannel(-1, ex, 0);
                game->explosion[game->explosionnum].x = game->man.x - 90;
                game->explosion[game->explosionnum].y = game->man.y - 60;
                game->explosion[game->explosionnum].tics = SDL_GetTicks();
                game->explosion[game->explosionnum].expolsion = 0;
                game->explosion[game->explosionnum].life = 'y';
                game->explosionnum += 1;

            }

            for (int i = 0; i < game->maxshots; i++) {
                if (game->shots[i].life == 'y') {
                    float sy = game->shots[i].y;
                    float sx = game->shots[i].x;
                    if (!(jx + iw < sx || jx > sx + 10 || jy > sy + 10 || jy + ih < sy) &&
                        game->fighter[j].life == 'y') {
                        game->fighter[j].life = 'e';
                        game->shots[i].life = 'n';
                        Mix_PlayChannel(-1, ex, 0);
                        game->explosion[game->explosionnum].x = game->fighter[i].x - 90;
                        game->explosion[game->explosionnum].y = game->fighter[i].y - 60;
                        game->explosion[game->explosionnum].expolsion = 0;
                        game->explosion[game->explosionnum].tics = SDL_GetTicks();
                        game->explosion[game->explosionnum].life = 'y';
                        game->explosionnum += 1;
                    }
                    if (!(wx + iw < sx || wx > sx + 10 || wy > sy + 10 || wy + ih < sy) &&
                        game->kam[j].life == 'y') {
                        game->kam[j].life = 'e';
                        game->shots[i].life = 'n';
                        Mix_PlayChannel(-1, ex, 0);
                        game->explosion[game->explosionnum].x = game->kam[j].x - 90;
                        game->explosion[game->explosionnum].y = game->kam[j].y - 60;
                        game->explosion[game->explosionnum].expolsion = 0;
                        game->explosion[game->explosionnum].tics = SDL_GetTicks();
                        game->explosion[game->explosionnum].life = 'y';
                        game->explosionnum += 1;
                    }
                    if (!(bx + iw < sx || bx > sx + 10 || by > sy + 10 || by + ih < sy) &&
                        game->bomber[j].life == 'y') {
                        game->bomber[j].life = 'e';
                        game->shots[i].life = 'n';
                        Mix_PlayChannel(-1, ex, 0);
                        game->explosion[game->explosionnum].x = game->bomber[j].x - 90;
                        game->explosion[game->explosionnum].y = game->bomber[j].y - 60;
                        game->explosion[game->explosionnum].expolsion = 0;
                        game->explosion[game->explosionnum].tics = SDL_GetTicks();
                        game->explosion[game->explosionnum].life = 'y';
                        game->explosionnum += 1;
                    }
                }

            }


        }
}

void doShot(GameState *game){
    for (int j = 0; j<game->maxenshots; j++) {
        if (game->enshots[j].life == 'y') {
            game->enshots[j].y += 10 * sin(game->enshots[j].r) * game->enshots[j].n;
            game->enshots[j].x += 10 * cos(game->enshots[j].r) * game->enshots[j].n;
        }
    }
    for (int i = 0; i<game->maxshots; i++){
        if (game->shots[i].x > 0){
            if (game->shots[i].r == 0) {
                game->shots[i].y -= 10;
            } else if (game->shots[i].r == 6) {
                game->shots[i].x -= 10;
            } else if (game->shots[i].r == 2) {
                game->shots[i].x += 10;
            }else if (game->shots[i].r == 4) {
                game->shots[i].y += 10;
            }else if (game->shots[i].r == 5) {
                game->shots[i].y += 10;
                game->shots[i].x -= 10;
            }else if (game->shots[i].r == 3) {
                game->shots[i].y += 10;
                game->shots[i].x += 10;
            }else if (game->shots[i].r == 1) {
                game->shots[i].y -= 10;
                game->shots[i].x += 10;
            }else if (game->shots[i].r == 7) {
                game->shots[i].y -= 10;
                game->shots[i].x -= 10;
            }
        }
        if (game->shots[i].x < -10 || game->shots[i].y < 0 || game->shots[i].x > game->width || game->shots[i].y > game->height){
            game->shots[i].life = 'n';
        }
    }
}

void hud(SDL_Renderer *renderer, GameState *game){
    SDL_Color White = {255, 255, 255};  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color
    //SDL_Surface* surfaceMessage = TTF_RenderText_Solid(game->ttf, "SCORE: 1000000000000000", White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
    //SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now you can convert it into a texture
    SDL_Rect Message_rect; //create a rect
    Message_rect.x = 90;  //controls the rect's x coordinate
    Message_rect.y = 0; // controls the rect's y coordinte
    Message_rect.w = 300; // controls the width of the rect
    Message_rect.h = 70; // controls the height of the rect;
    //SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
    //SDL_Surface* sMessage = TTF_RenderText_Solid(game->ttf, SDL_GetTicks(), White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
    //SDL_Texture* M = SDL_CreateTextureFromSurface(renderer, sMessage); //now you can convert it into a texture
    SDL_Rect M_rect; //create a rect
    M_rect.x = 490;  //controls the rect's x coordinate
    M_rect.y = 0; // controls the rect's y coordinte
    M_rect.w = 100; // controls the width of the rect
    M_rect.h = 70; // controls the height of the rect
   // SDL_RenderCopy(renderer, M, NULL, &M_rect);
    //free(c);

}

void doRender(SDL_Renderer *renderer, GameState *game) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    hud(renderer, game);
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_Rect green_rect = {random() % game->width, random() % game->height, 3, 3};
        SDL_RenderFillRect(renderer, &green_rect);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect red_rect = {random() % game->width, random() % game->height, 3, 3};
        SDL_RenderFillRect(renderer, &red_rect);
        game->tics = SDL_GetTicks();
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_Rect blue_rect = {random() % game->width, random() % game->height, 3, 3};
        SDL_RenderFillRect(renderer, &blue_rect);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect white_rect = {random() % game->width, random() % game->height, 3, 3};
        SDL_RenderFillRect(renderer, &white_rect);
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (game->man.life == 'y') {
        SDL_Rect rect_play = {game->man.x, game->man.y, 40, 40};
        SDL_RenderCopyEx(renderer, game->img, NULL, &rect_play, game->rot, NULL, flip);
    }
    for (int j = 0; j < game->maxenshots; j++) {
        if (game->enshots[j].x > 0 && game->enshots[j].life == 'y') {
            SDL_Rect rect_shot = {game->enshots[j].x, game->enshots[j].y, 10, 10};
            SDL_RenderCopyEx(renderer, game->shotim, NULL, &rect_shot, game->enshots[j].r, NULL, flip);
        }
    }
    for (int j = 0; j < game->maxmines; j++) {
        if (game->mine[j].x > 0 && game->mine[j].life == 'y') {
            SDL_Rect rect_shot = {game->mine[j].x, game->mine[j].y, 25, 25};
            SDL_RenderCopyEx(renderer, game->mineim, NULL, &rect_shot, 0, NULL, flip);
        }
    }
    double angle = 0;
    for (int i = 0; i < game->maxshots; i++) {
        if (game->shots[i].x > 0 && game->shots[i].life == 'y') {
            SDL_Rect rect_shot = {game->shots[i].x, game->shots[i].y, 10, 10};
            SDL_RenderCopyEx(renderer, game->shotim, NULL, &rect_shot, game->rot, NULL, flip);
        }
    }
    for (int i = 0; i < game->maxkam; i++) {
        if (game->kam[i].life == 'y') {
            angle = atan((game->kam[i].y - game->man.y) / (game->kam[i].x - game->man.x));
            angle = angle * (180 / 3.14);
            if (game->man.x > game->kam[i].x) {
                angle -= 90;
            } else if (game->man.x < game->kam[i].x) {
                angle += 90;
            }
            SDL_Rect rect_shot = {game->kam[i].x, game->kam[i].y, 40, 40};
            SDL_RenderCopyEx(renderer, game->kamim, NULL, &rect_shot, angle, NULL, flip);
        }
    }
    for (int i = 0; i < game->maxbombers; i++) {
        if (game->bomber[i].life == 'y') {
            angle = atan((game->bomber[i].y - game->man.y) / (game->bomber[i].x - game->man.x));
            angle = angle * (180 / 3.14);
            if (game->man.x > game->bomber[i].x) {
                angle -= 90;
            } else if (game->man.x < game->bomber[i].x) {
                angle += 90;
            }
            SDL_Rect rect_shot = {game->bomber[i].x, game->bomber[i].y, 40, 40};
            SDL_RenderCopyEx(renderer, game->bomberim, NULL, &rect_shot, angle, NULL, flip);
        }
    }
        for (int i = 0; i < game->maxfighter; i++) {
            if (game->fighter[i].life == 'y') {
                angle = atan((game->fighter[i].y - game->man.y) / (game->fighter[i].x - game->man.x));
                angle = angle * (180 / 3.14);
                if (game->man.x > game->fighter[i].x) {
                    angle -= 90;
                } else if (game->man.x < game->fighter[i].x) {
                    angle += 90;
                }
                SDL_Rect rect_shot = {game->fighter[i].x, game->fighter[i].y, 40, 40};
                SDL_RenderCopyEx(renderer, game->fightim, NULL, &rect_shot, angle, NULL, flip);
            }
        }
        for (int i = 0; i<game->maxexplosion; i++){
            if (game->explosion[i].life == 'y') {
                SDL_Rect rect_shot = {game->explosion[i].x - 90, game->explosion[i].y, 150, 150};
                if (game->explosion[i].expolsion == 0) {
                    SDL_RenderCopyEx(renderer, game->b1, NULL, &rect_shot, 0, NULL, flip);
                    int dt = SDL_GetTicks() - game->explosion[i].tics;
                    if (dt >= 900) {
                        game->explosion[i].expolsion += 1;
                    }
                } else if (game->explosion[i].expolsion == 1) {
                    SDL_RenderCopyEx(renderer, game->b2, NULL, &rect_shot, 0, NULL, flip);
                    int dt = SDL_GetTicks() - game->explosion[i].tics;
                    if (dt >= 900) {
                        game->explosion[i].expolsion += 1;
                    }
                } else if (game->explosion[i].expolsion == 2) {
                    SDL_RenderCopyEx(renderer, game->b3, NULL, &rect_shot, 0, NULL, flip);
                    int dt = SDL_GetTicks() - game->explosion[i].tics;
                    if (dt >= 900) {
                        game->explosion[i].expolsion += 1;
                    }
                } else if (game->explosion[i].expolsion == 3) {
                    SDL_RenderCopyEx(renderer, game->b4, NULL, &rect_shot, 0, NULL, flip);
                    int dt = SDL_GetTicks() - game->explosion[i].tics;
                    if (dt >= 900) {
                        game->explosion[i].expolsion += 1;
                    }
                } else if (game->explosion[i].expolsion == 4) {
                    SDL_RenderCopyEx(renderer, game->b5, NULL, &rect_shot, 0, NULL, flip);
                    int dt = SDL_GetTicks() - game->explosion[i].tics;
                    if (dt >= 900) {
                        game->explosion[i].expolsion += 1;
                    }
                } else if (game->explosion[i].expolsion == 5) {
                    SDL_RenderCopyEx(renderer, game->b6, NULL, &rect_shot, 0, NULL, flip);
                    int dt = SDL_GetTicks() - game->explosion[i].tics;
                    if (dt >= 900) {
                        game->explosion[i].expolsion += 1;
                    }
                } else if (game->explosion[i].expolsion == 6) {
                    SDL_RenderCopyEx(renderer, game->b7, NULL, &rect_shot, 0, NULL, flip);
                    int dt = SDL_GetTicks() - game->explosion[i].tics;
                    if (dt >= 900) {
                        game->explosion[i].expolsion += 1;
                    }
                } else if (game->explosion[i].expolsion == 7) {
                    SDL_RenderCopyEx(renderer, game->b8, NULL, &rect_shot, 0, NULL, flip);
                    int dt = SDL_GetTicks() - game->explosion[i].tics;
                    if (dt >= 900) {
                        game->explosion[i].expolsion += 1;
                    }
                } else if (game->explosion[i].expolsion == 8) {
                    SDL_RenderCopyEx(renderer, game->b9, NULL, &rect_shot, 0, NULL, flip);
                    int dt = SDL_GetTicks() - game->explosion[i].tics;
                    if (dt >= 900) {
                        game->explosion[i].expolsion += 1;
                    }
                } else if (game->explosion[i].expolsion == 9) {
                    SDL_RenderCopyEx(renderer, game->b10, NULL, &rect_shot, 0, NULL, flip);
                    int dt = SDL_GetTicks() - game->explosion[i].tics;
                    if (dt >= 900) {
                        game->explosion[i].expolsion += 1;
                    }
                } else if (game->explosion[i].expolsion == 10) {
                    SDL_RenderCopyEx(renderer, game->b11, NULL, &rect_shot, 0, NULL, flip);
                    int dt = SDL_GetTicks() - game->explosion[i].tics;
                    if (dt >= 900) {
                        game->explosion[i].expolsion += 1;
                    }
                    for (int j = 0; j < game->maxmines; j ++){
                        if (game->mine[j].life == 'e'){
                            game->mine[j].life = 'n';
                        }
                    }
                }
            }

        }
    for (int j = 0; j < game->max_bigass; j++) {
        if (game->big_astroid[j].life == 'y') {
            SDL_Rect rect_play = {game->big_astroid[j].x, game->big_astroid[j].y, 80, 80};
            SDL_RenderCopyEx(renderer, game->bigass_im, NULL, &rect_play, 0, NULL, flip);
        }
    }
    for (int j = 0; j < game->max_smallass; j++) {
        if (game->small_astroid[j].life == 'y') {
            if (game->small_astroid[j].n == 1) {
                SDL_Rect rect_play = {game->small_astroid[j].x, game->small_astroid[j].y, 40, 40};
                SDL_RenderCopyEx(renderer, game->smallass1_im, NULL, &rect_play, 0, NULL, flip);
            } else if (game->small_astroid[j].n == 2) {
                SDL_Rect rect_play = {game->small_astroid[j].x, game->small_astroid[j].y, 40, 40};
                SDL_RenderCopyEx(renderer, game->smallass2_im, NULL, &rect_play, 0, NULL, flip);
            }else if (game->small_astroid[j].n == 3){
                SDL_Rect rect_play = {game->small_astroid[j].x, game->small_astroid[j].y, 40, 40};
                SDL_RenderCopyEx(renderer, game->smallass3_im, NULL, &rect_play, 0, NULL, flip);
            } else if (game->small_astroid[j].n == 4) {
                SDL_Rect rect_play = {game->small_astroid[j].x, game->small_astroid[j].y, 40, 40};
                SDL_RenderCopyEx(renderer, game->smallass4_im, NULL, &rect_play, 0, NULL, flip);
            }
        }
    }
        SDL_RenderPresent(renderer);
    }

void loadround(GameState *game, int round){
    int fdt = SDL_GetTicks()-game->fighttic;
    int kdt = SDL_GetTicks()-game->kamtic;
    int bdt = SDL_GetTicks()-game->bombertic;
    if (round == 1){
        if (game->fightnum < 2 && fdt >= 2000){
            game->fighter[game->fightnum].x = -10000;
            game->fighter[game->fightnum].y = random()%game->height;
            game->fighter[game->fightnum].life = 'y';
            game->fightnum += 1;
            game->fighttic = SDL_GetTicks();
        } if (game->kamnum < 3 && kdt >= 500){
            game->kam[game->kamnum].x = -10000;
            game->kam[game->kamnum].y = random()%game->height;
            game->kam[game->kamnum].life = 'y';
            game->kamtic = SDL_GetTicks();
            game->kamnum += 1;
        }
        if (game->bombernum < 3 && bdt >= 1000){
            int dt = (random()%4);
            if (dt == 0){
                dt+=1;
            }
            game->bomber[game->bombernum].x = -1000;
            game->bomber[game->bombernum].y = random()%game->height;
            game->bomber[game->bombernum].life = 'y';
            game->bomber[game->bombernum].tics = SDL_GetTicks();
            game->bomber[game->bombernum].dt = dt*6000;
            game->bombertic = SDL_GetTicks();
            game->bombernum += 1;
        }
    }
}

void load(GameState *game, SDL_Renderer *renderer,  char *path, Mix_Music *space){
    Mix_PlayMusic(space, -1);
    SDL_Surface *surface = NULL;
    SDL_Surface *shot_img = NULL;
    SDL_Surface *fight_img = NULL;
    SDL_Surface *bomber_img = NULL;
    SDL_Surface *kam_img = NULL;
    SDL_Surface *mine_img = NULL;
    SDL_Surface *b1 = NULL;
    SDL_Surface *b2 = NULL;
    SDL_Surface *b3 = NULL;
    SDL_Surface *b4 = NULL;
    SDL_Surface *b5 = NULL;
    SDL_Surface *b6 = NULL;
    SDL_Surface *b7 = NULL;
    SDL_Surface *b8 = NULL;
    SDL_Surface *b9= NULL;
    SDL_Surface *b10 = NULL;
    SDL_Surface *b11 = NULL;
    SDL_Surface *bigass_im = NULL;
    SDL_Surface *small1_im = NULL;
    SDL_Surface *small2_im = NULL;
    SDL_Surface *small3_im = NULL;
    SDL_Surface *small4_im = NULL;
    game->fighttic = SDL_GetTicks();
    game->kamtic = SDL_GetTicks();
    game->bombertic = SDL_GetTicks();
    game->tics = SDL_GetTicks();
    game->asstic = SDL_GetTicks();
    game->kamnum = 0;
    game->fightnum = 0;
    game->bombernum = 0;
    game->minenum = 0;
    game->explosionnum = 0;
    game->man.x = game->width/2;
    game->man.y = game->height/2;
    game->man.w = 40;
    game->man.h = 40;
    game->man.life = 'y';
    game->fighter->w = 40;
    game->fighter->h = 40;
    game->small_astroid->w = 40;
    game->mine->w = 25;
    game->mine->h = 25;
    game->big_astroid->w = 80;
    game->shots->w = 10;
    game->explosionnum = 0;
    game->shots->h = 10;
    game->rot = 0.00f;
    game->shotsnum = 0;
    game->shotem = 0;
    game->maxfighter = 40;
    game->maxkam = 40;
    game->maxshots = 1000;
    game->maxexplosion = 100;
    game->maxenshots = 1000;
    game->maxmines = 50;
    game->maxbombers = 40;
    game->max_bigass = 20;
    game->max_smallass = 80;
    game->assnum = 0;
    game->smallassnum = 0;
    for (int i = 0; i < game->maxfighter; i++){
        game->fighter[i].x = -100000;
        game->kam[i].x = -100000;
        game->fighter[i].life = 'n';
        game->kam[i].life = 'n';
        game->bomber[i].x = -100000;
        game->bomber[i].life = 'n';
        game->bomber[i].movex = 0;
        game->bomber[i].movey = 0;
    }
    for (int i = 0; i<game->maxshots; i++){
        game->shots[i].x = -1;
        game->shots[i].life = 'n';
    }
    for (int i = 0; i<game->maxmines; i++){
        game->mine[i].x = -700;
        game->mine[i].life = 'n';
    }
    for (int i = 0; i<game->maxenshots; i++){
        game->enshots[i].x = -1;
        game->enshots[i].life = 'n';
    }
    for (int i = 0; i<game->max_smallass; i++){
        game->small_astroid[i].x = -1;
        game->small_astroid
        [i].life = 'n';
    }
    for (int i = 0; i<game->max_bigass; i++){
        game->big_astroid[i].x = -1;
        game->big_astroid[i].life = 'n';
    }
    char *p = path;
    surface = IMG_Load(strcat(p,"//ship1.png"));
    p[strlen(p)-11] = 0;
    shot_img = IMG_Load(strcat(p,"//shotsmall.png"));
    p[strlen(p)-15] = 0;
    fight_img = IMG_Load(strcat(p,"//fighter.png"));
    p[strlen(p)-13] = 0;
    kam_img = IMG_Load(strcat(p,"//kamikaze.png"));
    p[strlen(p)-14] = 0;
    b1 = IMG_Load(strcat(p,"/explosion//boom01.png"));
    p[strlen(p)-22] = 0;
    b2 = IMG_Load(strcat(p,"/explosion//boom02.png"));
    p[strlen(p)-22] = 0;
    b3 = IMG_Load(strcat(p,"/explosion//boom03.png"));
    p[strlen(p)-22] = 0;
    b4 = IMG_Load(strcat(p,"/explosion//boom04.png"));
    p[strlen(p)-22] = 0;
    b5 = IMG_Load(strcat(p,"/explosion//boom05.png"));
    p[strlen(p)-22] = 0;
    b6 = IMG_Load(strcat(p,"/explosion//boom06.png"));
    p[strlen(p)-22] = 0;
    b7 = IMG_Load(strcat(p,"/explosion//boom07.png"));
    p[strlen(p)-22] = 0;
    b8 = IMG_Load(strcat(p,"/explosion//boom08.png"));
    p[strlen(p)-22] = 0;
    b9 = IMG_Load(strcat(p,"/explosion//boom09.png"));
    p[strlen(p)-22] = 0;
    b10 = IMG_Load(strcat(p,"/explosion//boom10.png"));
    p[strlen(p)-22] = 0;
    b11 = IMG_Load(strcat(p,"/explosion//boom11.png"));
    p[strlen(p)-22] = 0;
    bomber_img = IMG_Load(strcat(p,"//boarder.png"));
    p[strlen(p)-13] = 0;
    mine_img = IMG_Load(strcat(p, "//mine0.png"));
    p[strlen(p)-11] = 0;
    bigass_im = IMG_Load(strcat(p, "//a.png"));
    p[strlen(p)-7] = 0;
    small1_im = IMG_Load(strcat(p, "//smalla1.png"));
    p[strlen(p)-13] = 0;
    small2_im = IMG_Load(strcat(p, "//smalla2.png"));
    p[strlen(p)-13] = 0;
    small3_im = IMG_Load(strcat(p, "//smalla3.png"));
    p[strlen(p)-13] = 0;
    small4_im = IMG_Load(strcat(p, "//smalla4.png"));
    p[strlen(p)-13] = 0;
    game->mineim = SDL_CreateTextureFromSurface(renderer, mine_img);
    game->shotim = SDL_CreateTextureFromSurface(renderer, shot_img);
    game->img = SDL_CreateTextureFromSurface(renderer, surface);
    game->fightim = SDL_CreateTextureFromSurface(renderer, fight_img);
    game->kamim = SDL_CreateTextureFromSurface(renderer, kam_img);
    game->bomberim = SDL_CreateTextureFromSurface(renderer, bomber_img);
    game->b1 = SDL_CreateTextureFromSurface(renderer, b1);
    game->b2 = SDL_CreateTextureFromSurface(renderer, b2);
    game->b3 = SDL_CreateTextureFromSurface(renderer, b3);
    game->b4 = SDL_CreateTextureFromSurface(renderer, b4);
    game->b5 = SDL_CreateTextureFromSurface(renderer, b5);
    game->b6 = SDL_CreateTextureFromSurface(renderer, b6);
    game->b7 = SDL_CreateTextureFromSurface(renderer, b7);
    game->b8 = SDL_CreateTextureFromSurface(renderer, b8);
    game->b9 = SDL_CreateTextureFromSurface(renderer, b9);
    game->b10 = SDL_CreateTextureFromSurface(renderer, b10);
    game->b11 = SDL_CreateTextureFromSurface(renderer, b11);
    game->bigass_im = SDL_CreateTextureFromSurface(renderer, bigass_im);
    game->smallass1_im = SDL_CreateTextureFromSurface(renderer, small1_im);
    game->smallass2_im = SDL_CreateTextureFromSurface(renderer, small2_im);
    game->smallass3_im = SDL_CreateTextureFromSurface(renderer, small3_im);
    game->smallass4_im = SDL_CreateTextureFromSurface(renderer, small4_im);
    SDL_FreeSurface(surface);
    SDL_FreeSurface(mine_img);
    SDL_FreeSurface(bomber_img);
    SDL_FreeSurface(shot_img);
    SDL_FreeSurface(fight_img);
    SDL_FreeSurface(kam_img);
    SDL_FreeSurface(b1);
    SDL_FreeSurface(b2);
    SDL_FreeSurface(b3);
    SDL_FreeSurface(b4);
    SDL_FreeSurface(b5);
    SDL_FreeSurface(b6);
    SDL_FreeSurface(b7);
    SDL_FreeSurface(b8);
    SDL_FreeSurface(b9);
    SDL_FreeSurface(b10);
    SDL_FreeSurface(b11);
    SDL_FreeSurface(small1_im);
    SDL_FreeSurface(small2_im);
    SDL_FreeSurface(small3_im);
    SDL_FreeSurface(small4_im);
    SDL_FreeSurface(bigass_im);
}

int alive(GameState *game){
    int fnum = 0;
    int knum = 0;
    int bnum = 0;
    for (int i = 0; i<game->maxfighter; i++){
        if(game->fighter[i].life == 'y'){
            fnum += 1;
        }
        if(game->kam[i].life == 'y'){
            knum += 1;
        }
        if(game->bomber[i].life == 'y'){
            bnum += 1;
        }
    }
    if(game->man.life != 'y'){
     return 1;
    }
    return 0;
    printf("Fighters: %d\n", fnum);
    printf("Kams: %d\n", knum);
    printf("Bombers: %d\n", bnum);
}

int main(int argc, char *argv[]){
    char *path = argv[0];
    //change to 10 so it can excute in the same dictorary as the image
    path[strlen(path)-10] = 0;
    GameState gameSate;
    gameSate.height = 720;
    gameSate.width = 1080;
    srandom((int)time(NULL));
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    IMG_Init(IMG_INIT_JPG);
    TTF_Init();
    SDL_Window *window= NULL;
    SDL_Renderer *renderer = NULL;
    window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, gameSate.width, gameSate.height, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window,-1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    Mix_Chunk *ex = Mix_LoadWAV(strcat(path,"/explosions.wav"));
    path[strlen(path)-15] = 0;
    Mix_Music *boss = Mix_LoadMUS(strcat(path,"/boss.mp3"));
    path[strlen(path)-9] = 0;
    Mix_Music *space = Mix_LoadMUS(strcat(path,"/space.mp3"));
    path[strlen(path)-10] = 0;
    Mix_Music *title = Mix_LoadMUS(strcat(path,"/title.mp3"));
    path[strlen(path)-9] = 0;
    Mix_Chunk *laser = Mix_LoadWAV(strcat(path,"/laser3.ogg"));
    path[strlen(path)-11] = 0;
    gameSate.laser = laser;
    Mix_Chunk *enlaser = Mix_LoadWAV(strcat(path,"/laser5.ogg"));
    path[strlen(path)-11] = 0;
    Mix_Chunk *aex = Mix_LoadWAV(strcat(path,"/aex.ogg"));
    path[strlen(path)-8] = 0;
    gameSate.ttf = TTF_OpenFont( strcat(path,"//space.ttf"), 70);
    path[strlen(path)-11] = 0;
    gameSate.enlaser = enlaser;
    load(&gameSate, renderer, path, space);
    int done = 0;
    int round = 1;
    while (!done){
        
        loadround(&gameSate, round);
        done = processEvents(window, &gameSate);
        done = alive(&gameSate);
        collison(&gameSate, ex, aex);
        doRender(renderer, &gameSate);
        doShot(&gameSate);
        moveEnemy(&gameSate);
        
    }
    Mix_FreeChunk(ex);
    Mix_FreeMusic(boss);
    Mix_FreeMusic(space);
    Mix_FreeChunk(enlaser);
    Mix_FreeChunk(aex);
    Mix_FreeChunk(laser);
    Mix_FreeMusic(title);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    Mix_Quit();
    SDL_Quit();
    TTF_Quit();
    return 0;
}
