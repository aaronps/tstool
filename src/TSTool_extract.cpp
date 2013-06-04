
#include "TSTool.hpp"
#include "ToolManager.hpp"

#include <iostream>
#include <SDL/SDL_video.h>

#include "SDL.h"
#include "SDL_png.h"

using std::string;

class TSTool_extract : public TSTool
{
public:
    void run(const std::vector<std::string>& params)
    {
        if ( params.size() < 5 )
        {
            std::cout << "Use: extract <image.png> <first_tile> <xtiles> <ytiles> <to.png>\n";
            return;
        }
        
        string timage = params[0];
        int first_tile = atoi(params[1].c_str());
        int xtiles = atoi(params[2].c_str());
        int ytiles = atoi(params[3].c_str());
        string destimage = params[4];
        
        SDL_Surface * image = SDL_LoadPNG(timage.c_str());
        if ( ! image )
        {
            std::cout << "Error loading tileset image " << timage << ": " << SDL_GetError() << "\n";
            return;
        }

        int row_tiles = image->w / 32;
        int rows = image->h / 32;
        
        SDL_Surface * dest = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                                  xtiles*32, ytiles*32,
                                                  image->format->BitsPerPixel,
                                                  image->format->Rmask,
                                                  image->format->Gmask,
                                                  image->format->Bmask,
                                                  image->format->Amask);

        if ( image->format->BitsPerPixel <= 8 )
        {
            SDL_SetColors(dest, image->format->palette->colors, 0, image->format->palette->ncolors);
        }
        
        SDL_FillRect(dest, 0, 0);
        
        
        SDL_Rect destr = {0, 0, 0, 0};
        SDL_Rect r = { 0, 0, 32, 32 };
        
        for ( int yt = 0; yt < ytiles; yt++ )
        {
            destr.x = 0;
            for (int xt = 0; xt < xtiles; xt++ )
            {
                int tsrow = first_tile / row_tiles;
                int tscol = first_tile % row_tiles;
                r.x = tscol * 32;
                r.y = tsrow * 32;
                
                SDL_BlitSurface(image, &r, dest, &destr);
                
                first_tile++;
                destr.x += 32;
            }
            destr.y += 32;
        }
        
        std::cout << "Next object tile is " << first_tile << "\n";
        
        SDL_SavePNG(dest, destimage.c_str());
        
        SDL_FreeSurface(dest);
        SDL_FreeSurface(image);
    }
};

static ToolManagerAutoRegister<TSTool_extract> tsextract("extract", "Extracts a subpicture from the tileset");
