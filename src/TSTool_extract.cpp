
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
        
        Tileset * sub = ts.subTileset(destbase, first_tile, xtiles*ytiles, xtiles );
        if ( sub )
        {
            sub->save(destbase);
            std::cout << "Next object tile is " << (first_tile + xtiles*ytiles) << "\n";
            delete sub;
        }
        else
        {
            std::cout << "Error extracting subtileset\n";
        }
    }
};

static ToolManagerAutoRegister<TSTool_extract> tsextract("extract", "Extracts a subpicture from the tileset");
