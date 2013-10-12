
#include "TSTool.hpp"
#include "ToolManager.hpp"
#include "Tileset.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <SDL/SDL_video.h>

#include "SDL.h"
#include "SDL_png.h"

using std::string;
using std::vector;

class TSTool_extract : public TSTool
{
public:
    void run(const std::vector<std::string>& params)
    {
        if ( params.size() < 5 )
        {
            std::cout << "Use: extract <tileset> <first_tile> <xtiles> <ytiles> <to_no_extension>\n";
            return;
        }
        
        int first_tile = atoi(params[1].c_str());
        int xtiles = atoi(params[2].c_str());
        int ytiles = atoi(params[3].c_str());
        string destbase = params[4];
        
        Tileset ts;
        if ( ! ts.load(params[0]) )
        {
            return;
        }

        const SDL_Surface * image = ts.image();
        
        int row_tiles = image->w / ts.tile_width();
        int rows = image->h / ts.tile_height();
        
        SDL_Surface * dest = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                                  xtiles*ts.tile_width(),
                                                  ytiles*ts.tile_height(),
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
        
        vector<uint8_t> moves;

        SDL_Rect destr = {0, 0, 0, 0};
        SDL_Rect r = { 0, 0, ts.tile_width(), ts.tile_height() };
        
        for ( int yt = 0; yt < ytiles; yt++ )
        {
            destr.x = 0;
            for (int xt = 0; xt < xtiles; xt++ )
            {
                moves.push_back(ts.tile_move(first_tile));
                int tsrow = first_tile / row_tiles;
                int tscol = first_tile % row_tiles;
                r.x = tscol * ts.tile_width();
                r.y = tsrow * ts.tile_height();
                
                SDL_BlitSurface(const_cast<SDL_Surface*>(image), &r, dest, &destr);
                
                first_tile++;
                destr.x += ts.tile_width();
            }
            destr.y += ts.tile_height();
        }
        
        std::cout << "Next object tile is " << first_tile << "\n";
        
        string destimage = destbase + ".png";
        SDL_SavePNG(dest, destimage.c_str());
        
        string destdata = destbase + ".json";
        std::ofstream out( destdata.c_str());
        
        out << "{\"move\": [\n" << int(moves[0]);
        for (int n = 1; n < moves.size(); n++ )
        {
            out << ((n%xtiles) ? "," : ",\n") << int(moves[n]);
        }
        
        out << "\n]}";
        out.close();
        
        SDL_FreeSurface(dest);
    }
};

static ToolManagerAutoRegister<TSTool_extract> tsextract("extract", "Extracts a subpicture from the tileset");
