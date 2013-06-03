
#include "TSTool.hpp"
#include "ToolManager.hpp"

#include <iostream>

#include "SDL.h"
#include "SDL_png.h"

class TSTool_bmp2png : public TSTool
{
public:
    void run(const std::vector<std::string>& params)
    {
        if ( params.size() < 2 )
        {
            std::cout << "Use: bmp2png <from.bmp> <to.png>\n";
            return;
        }
        
        SDL_Surface * image = SDL_LoadBMP(params[0].c_str());
        if ( ! image )
        {
            std::cout << "Error loading image " << params[0] << ": " << SDL_GetError() << "\n";
            return;
        }

        if ( SDL_SavePNG(image, params[1].c_str()) == -1 )
        {
            std::cout << "Error saving file " << params[1] << ": " << SDL_GetError() << "\n";
        }
        else
        {
            std::cout << "Saved file " << params[1] << "\n";
        }
        SDL_FreeSurface(image);
    }
};

class TSTool_png2bmp : public TSTool
{
public:
    void run(const std::vector<std::string>& params)
    {
        if ( params.size() < 2 )
        {
            std::cout << "Use: png2bmp <from.png> <to.bmp>\n";
            return;
        }
        
        SDL_Surface * image = SDL_LoadPNG(params[0].c_str());
        if ( ! image )
        {
            std::cout << "Error loading image " << params[0] << ": " << SDL_GetError() << "\n";
            return;
        }

        if ( SDL_SaveBMP(image, params[1].c_str()) == -1 )
        {
            std::cout << "Error saving file " << params[1] << ": " << SDL_GetError() << "\n";
        }
        else
        {
            std::cout << "Saved file " << params[1] << "\n";
        }
        SDL_FreeSurface(image);
    }
};

static ToolManagerAutoRegister<TSTool_bmp2png> bmp2png("bmp2png", "Converts bmp images to png format");

static ToolManagerAutoRegister<TSTool_png2bmp> png2bmp("png2bmp", "Converts png images to bmp format");
