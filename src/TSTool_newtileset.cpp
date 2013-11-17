
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

class TSTool_newtileset : public TSTool
{
public:
    void run(const std::vector<std::string>& params)
    {
        if ( params.size() < 4 )
        {
            std::cout << "Use: new <tileset> <\"header text\"> <firstsubtileset> <tiles per row>\n";
            return;
        }
        
        const string tsname = params[0];
        const string tsheader = params[1];
        const string subtsname = params[2];
        const unsigned tpr = atoi(params[3].c_str());

        Tileset subts;
        if ( ! subts.load(subtsname) )
        {
            std::cout << "Error, cannot load " << subtsname << "\n";
            return;
        }
        
        const unsigned tsw = subts.tile_width();
        const unsigned tsh = subts.tile_height();
        
        Tileset ts(tsheader, tsw, tsh);
        ts.appendTilesFrom(subts, tpr);
        ts.save(tsname);
        
        std::cout << "Maybe is ok, bye\n";
    }
};

static ToolManagerAutoRegister<TSTool_newtileset> tsnew("new", "Creates a new, empty tileset");
