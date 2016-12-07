//
//  main.c
//  Pacman
//
//  Created by Palou & Audrey J on 07/12/2016.
//  Copyright © 2016 Pascal Jacquemart. All rights reserved.
//

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <Lua/lua.h>
#include <Lua/lauxlib.h>
#include <Lua/lualib.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

typedef struct __context__ {
    /* SDL related objects */
    SDL_Window* window;
    SDL_Surface* screenSurface;
    SDL_Rect rect;

    /* Lua contexts */
    lua_State *L;
} context_t;


context_t *load_lua_runtime();
int lua_ext_draw_rectangle (lua_State *L);
int lua_ext_get_key_input (lua_State *L);
void error (lua_State *L, const char *fmt, ...);
int lua_run(context_t * ctx);

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
            ctx->screenSurface = SDL_GetWindowSurface( ctx->window );

            SDL_FillRect( ctx->screenSurface, NULL, SDL_MapRGB( ctx->screenSurface->format, 0x00, 0x00, 0x00 ) );
            SDL_UpdateWindowSurface( ctx->window );

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
    lua_pushcfunction(L, lua_ext_get_key_input);
    lua_setglobal(L, "getKey");

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
    lua_getglobal(L, "ctx");
    ctx = (context_t*)lua_touserdata(L, -1);

    lua_getfield(L, 1, "left");
    lua_getfield(L, 1, "top");
    lua_getfield(L, 1, "width");
    lua_getfield(L, 1, "height");

    ctx->rect.x = lua_tonumber(L, -1);
    ctx->rect.y = lua_tonumber(L, -2);
    ctx->rect.w = lua_tonumber(L, -3);
    ctx->rect.h = lua_tonumber(L, -4);

    printf("drawRect x=%d, y=%d, w=%d, h=%d\n", ctx->rect.x, ctx->rect.y, ctx->rect.w, ctx->rect.h);

    SDL_FillRect( ctx->screenSurface, &ctx->rect, SDL_MapRGB( ctx->screenSurface->format, 0xFF, 0xFF, 0xFF ) );
    SDL_UpdateWindowSurface( ctx->window );
    lua_pushboolean(L, 1);
    return 1;  /* number of results */
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
            case SDL_KEYUP:
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

