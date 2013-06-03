/* 
 * File:   tstool.cpp
 * Author: admin
 *
 * Created on June 1, 2013, 2:19 PM
 */

#include "SDL.h"
#undef main

#include <windows.h>

#include <stdio.h>

bool miniSDLInit()
{
    if ( SDL_Init(SDL_INIT_NOPARACHUTE) < 0 )
    {
        return false;
    }

    SDL_SetModuleHandle(GetModuleHandle(NULL));
    
    atexit(SDL_Quit);
    
    return true;
}

void print_usage(const char * exe_name)
{
    printf("Use: %s <tileset-image-file>\n", exe_name);
}

/*
 * 
 */
int main(int argc, char** argv)
{
    printf("TileSet Tool 0.1\n");
    if ( ! miniSDLInit() )
    {
        printf("Error initializing SDL\n");
        return 1;
    }
    
    if ( argc < 2 )
    {
        print_usage(argv[0]);
        return 0;
    }
    
    const char * tileset_file = argv[1];
    
    SDL_Surface * tileset = IMG_Load(tileset_file);
    if ( ! tileset )
    {
        printf("Error loading tileset image file '%s': %s\n", tileset_file, IMG_GetError());
        return 1;
    }
    
    
    
    SDL_FreeSurface(tileset);
    
    return 0;
}

