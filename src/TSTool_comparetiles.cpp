
#include "TSTool.hpp"
#include "ToolManager.hpp"

#include <iostream>
#include <SDL/SDL_video.h>

#include "SDL.h"
#include "SDL_png.h"

using std::string;

class TSTool_compare : public TSTool
{
public:
    void run(const std::vector<std::string>& params)
    {
        if ( params.size() < 1 )
        {
            std::cout << "Use: compare <image.png>\n";
            return;
        }
        
        string timage = params[0];
        SDL_Surface * image = SDL_LoadPNG(timage.c_str());
        if ( ! image )
        {
            std::cout << "Error loading tileset image " << timage << ": " << SDL_GetError() << "\n";
            return;
        }

        int row_tiles = image->w / 32;
        int rows = image->h / 32;
        
        int total_tiles = rows * row_tiles;
        
        
//        SDL_Rect destr = {0, 0, 0, 0};
//        SDL_Rect r = { 0, 0, 32, 32 };
//        
//        for ( int yt = 0; yt < ytiles; yt++ )
//        {
//            destr.x = 0;
//            for (int xt = 0; xt < xtiles; xt++ )
//            {
//                int tsrow = first_tile / row_tiles;
//                int tscol = first_tile % row_tiles;
//                r.x = tscol * 32;
//                r.y = tsrow * 32;
//                
//                SDL_BlitSurface(image, &r, dest, &destr);
//                
//                first_tile++;
//                destr.x += 32;
//            }
//            destr.y += 32;
//        }
        
        SDL_FreeSurface(image);
    }
};

//static ToolManagerAutoRegister<TSTool_compare> tscompare("comparetiles", "Compares all the tiles on the tileset to find repeated ones");
