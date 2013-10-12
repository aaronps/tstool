
#include "TSTool.hpp"
#include "ToolManager.hpp"
#include "Tileset.hpp"

#include <iostream>
#include <SDL/SDL_video.h>

using std::string;

class TSTool_deletetiles : public TSTool
{
public:
    void run(const std::vector<std::string>& params)
    {
        if ( params.size() < 2 )
        {
            std::cout << "Use: deletetiles <tileset> <first_tile> [num_to_delete=1]\n";
            return;
        }
        
        Tileset ts;
        if ( ! ts.load(params[0]) )
        {
            return;
        }
        
        int first_tile = atoi(params[1].c_str());
        if ( first_tile > 0 )
        {
            first_tile--;
        }
        
        int to_delete = 1;
        if ( params.size() > 2 )
        {
            to_delete = atoi(params[2].c_str());
        }
        
        ts.deleteTiles(first_tile, to_delete);
        
        ts.save(params[0]);
    }
};

static ToolManagerAutoRegister<TSTool_deletetiles> tsdelete("deletetiles", "Delete one or more tiles");
