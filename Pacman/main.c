//
//  main.c
//  Pacman
//
//  Created by Palou & Audrey J on 07/12/2016.
//  Copyright © 2016 Pascal Jacquemart. All rights reserved.
//

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <Lua/lua.h>
#include <Lua/lauxlib.h>
#include <Lua/lualib.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

typedef struct __context__ {
    /* SDL related objects */
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Rect rect;
    SDL_Rect src;
    SDL_Rect dst;

    /* Lua contexts */
    lua_State *L;
} context_t;


context_t *load_lua_runtime();
int lua_ext_draw_rectangle (lua_State *L);
int lua_ext_draw_sprite (lua_State *L);
int lua_ext_refresh_screen (lua_State *L);
int lua_ext_get_key_input (lua_State *L);
int lua_ext_usleep (lua_State *L);
void error (lua_State *L, const char *fmt, ...);
int lua_run(context_t *ctx);

int main( int argc, char* args[] )
{
    context_t *ctx = load_lua_runtime();
    
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    }
    else
    {
        ctx->window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( ctx->window == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        }
        else
        {
            ctx->renderer = SDL_CreateRenderer(ctx->window, -1, SDL_RENDERER_ACCELERATED);
            if (ctx->renderer == NULL) {
                printf("SDL_CreateRenderer failed: %s\n", SDL_GetError());
            }

            //Initialize PNG loading
            int imgFlags = IMG_INIT_PNG;
            if( !( IMG_Init( imgFlags ) & imgFlags ) )
            {
                printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
            }
            SDL_Surface *surface = IMG_Load("original_spritesheet.png");
            printf("Sprites surface: %p\n", surface);
            ctx->texture = SDL_CreateTextureFromSurface(ctx->renderer, surface);
            if (ctx->texture == NULL) {
                printf("CreateTextureFromSurface failed: %s\n", SDL_GetError());
            }
            SDL_FreeSurface(surface);
            printf("Sprites texture: %p\n", ctx->texture);

            lua_run(ctx);
            SDL_Delay(2000);

            // SDL has to cleanup before because the context will be lost after lua_close()
            SDL_DestroyWindow( ctx->window );
            SDL_Quit();

            lua_close(ctx->L);
        }
    }
    
    return 0;
}

context_t* load_lua_runtime()
{
    context_t *ctx = NULL;
    lua_State *L = luaL_newstate();   /* opens Lua */
    luaL_openlibs(L);
    if (luaL_loadfile(L, "pacman.lua")) {
        error(L, "cannot load configuration file: %s",
              lua_tostring(L, -1));
    }
    ctx = (context_t *)lua_newuserdata (L, sizeof(context_t));
    ctx->L = L;
    lua_setglobal(L, "ctx");
    lua_pushcfunction(L, lua_ext_draw_rectangle);
    lua_setglobal(L, "drawRect");
    lua_pushcfunction(L, lua_ext_draw_sprite);
    lua_setglobal(L, "drawSprite");
    lua_pushcfunction(L, lua_ext_refresh_screen);
    lua_setglobal(L, "refreshScreen");
    lua_pushcfunction(L, lua_ext_get_key_input);
    lua_setglobal(L, "getKey");
    lua_pushcfunction(L, lua_ext_usleep);
    lua_setglobal(L, "usleep");

    return ctx;
}

int lua_run(context_t * ctx)
{
    if (lua_pcall(ctx->L, 0, 0, 0)) {
        error(ctx->L, "cannot run configuration file: %s",
              lua_tostring(ctx->L, -1));
    }

    return 0;
}

int lua_ext_draw_rectangle (lua_State *L)
{
    context_t *ctx = NULL;
    const char *str;
    lua_getglobal(L, "ctx");
    ctx = (context_t*)lua_touserdata(L, -1);

    lua_getfield(L, 1, "left");
    lua_getfield(L, 1, "top");
    lua_getfield(L, 1, "width");
    lua_getfield(L, 1, "height");
    lua_getfield(L, 1, "color");

    ctx->rect.x = lua_tonumber(L, -5);
    ctx->rect.y = lua_tonumber(L, -4);
    ctx->rect.w = lua_tonumber(L, -3);
    ctx->rect.h = lua_tonumber(L, -2);
    str = lua_tostring(L, -1);
    
    //printf("drawRect x=%d, y=%d, w=%d, h=%d\n", ctx->rect.x, ctx->rect.y, ctx->rect.w, ctx->rect.h);

    if (!strcmp(str, "grey")) {
        //color = SDL_MapRGB( ctx->screenSurface->format, 0x5F, 0x5F, 0x5F );
        SDL_SetRenderDrawColor(ctx->renderer, 0x5F, 0x5F, 0x5F, 0xFF);
    } else if (!strcmp(str, "yellow")) {
        //color= SDL_MapRGB( ctx->screenSurface->format, 0xFF, 0xD7, 0x00 );
        SDL_SetRenderDrawColor(ctx->renderer, 0xFF, 0xD7, 0x00, 0xFF);
    } else if (!strcmp(str, "red")) {
        //color= SDL_MapRGB( ctx->screenSurface->format, 0x7F, 0x0F, 0x0F );
        SDL_SetRenderDrawColor(ctx->renderer, 0x7F, 0x0F, 0x0F, 0xFF);
    } else if (!strcmp(str, "green")) {
        //color= SDL_MapRGB( ctx->screenSurface->format, 0x0F, 0x7F, 0x0F );
        SDL_SetRenderDrawColor(ctx->renderer, 0x0F, 0x7F, 0x0F, 0xFF);
    } else {
        //color = SDL_MapRGB( ctx->screenSurface->format, 0x00, 0x00, 0x00 );
        SDL_SetRenderDrawColor(ctx->renderer, 0x00, 0x00, 0x00, 0xFF);
    }

    SDL_RenderFillRect(ctx->renderer, &ctx->rect);

    lua_pushboolean(L, 1);
    return 1;  /* number of results */
}

int lua_ext_refresh_screen (lua_State *L)
{
    context_t *ctx = NULL;
    lua_getglobal(L, "ctx");
    ctx = (context_t*)lua_touserdata(L, -1);
    SDL_RenderPresent(ctx->renderer);
    lua_pushboolean(L, 1);
    return 1;  /* number of results */
}

int lua_ext_draw_sprite (lua_State *L)
{
    context_t *ctx = NULL;
    lua_getglobal(L, "ctx");
    ctx = (context_t*)lua_touserdata(L, -1);

    // Src rect
    lua_getfield(L, 1, "src_left");
    lua_getfield(L, 1, "src_top");
    lua_getfield(L, 1, "src_width");
    lua_getfield(L, 1, "src_height");

    // Dest rect
    lua_getfield(L, 1, "dst_left");
    lua_getfield(L, 1, "dst_top");
    lua_getfield(L, 1, "dst_width");
    lua_getfield(L, 1, "dst_height");

    ctx->src.x = lua_tonumber(L, -8);
    ctx->src.y = lua_tonumber(L, -7);
    ctx->src.w = lua_tonumber(L, -6);
    ctx->src.h = lua_tonumber(L, -5);
    ctx->dst.x = lua_tonumber(L, -4);
    ctx->dst.y = lua_tonumber(L, -3);
    ctx->dst.w = lua_tonumber(L, -2);
    ctx->dst.h = lua_tonumber(L, -1);

    //printf("src Rect x=%d, y=%d, w=%d, h=%d\n", ctx->src.x, ctx->src.y, ctx->src.w, ctx->src.h);
    //printf("dst Rect x=%d, y=%d, w=%d, h=%d\n", ctx->dst.x, ctx->dst.y, ctx->dst.w, ctx->dst.h);

    SDL_RenderCopy(ctx->renderer, ctx->texture, &ctx->src, &ctx->dst);

    lua_pushboolean(L, 1);
    return 1;  /* number of results */
}

int lua_ext_usleep (lua_State *L)
{
    double d = luaL_checknumber(L, 1);
    usleep(d);
    return 0;
}

int lua_ext_get_key_input (lua_State *L)
{
    SDL_Event event;
    const char *key = NULL;
    int count = 0;

    /* Poll for events. SDL_PollEvent() returns 0 when there are no  */
    /* more events on the event queue, our while loop will exit when */
    /* that occurs.                                                  */
    while( SDL_PollEvent( &event ) ){
        /* We are only worried about SDL_KEYDOWN and SDL_KEYUP events */
        switch( event.type ){
            case SDL_KEYDOWN:
                key = SDL_GetKeyName(event.key.keysym.sym);
                //printf("Key: %s\n", key);
                count ++;
                lua_pushstring(L, key);
                break;
                
            default:
                break;
        }
    }
    return count;
}

void error (lua_State *L, const char *fmt, ...)
{
    va_list argp;
    va_start(argp, fmt);
    vfprintf(stderr, fmt, argp);
    va_end(argp);
    lua_close(L);
    exit(EXIT_FAILURE);
}

