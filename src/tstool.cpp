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
#include <iostream>

#include "ToolManager.hpp"

using std::vector;
using std::string;

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
    printf("Use: %s <tool_name> <parameters>\n", exe_name);
}

/*
 * 
 */
int main(int argc, char** argv)
{
    printf("TileSet Tool 0.3\n");
    if ( ! miniSDLInit() )
    {
        printf("Error initializing SDL\n");
        return 1;
    }
    
    if ( argc < 2 )
    {
        print_usage(argv[0]);
        ToolManager::showHelp(std::cout);
        return 0;
    }
    
    vector<string> params;
    for ( int n = 2; n < argc; n++ )
    {
        params.push_back(argv[n]);
    }
    
    if ( ToolManager::hasTool(argv[1]) )
    {
        ToolManager::runTool(argv[1], params);
    }
    else
    {
        ToolManager::showHelp(std::cout);
    }
    
    return 0;
}

