#include "TSTool.hpp"
#include "ToolManager.hpp"

#include <iostream>
#include <fstream>
#include <stdio.h>

#include "Tileset.hpp"
#include "json.h"

class TSTool_append : public TSTool
{
public:
    void run(const std::vector<std::string>& params)
    {
        if ( params.size() < 2 )
        {
            std::cout << "Use: append <tileset> <new_tiles>\n";
            return;
        }
        
        std::string tsname = params[0];
        std::string newbase = params[1];
        
        Tileset ts;
        if ( ! ts.load(tsname) )
        {
            std::cout << "Can't load the tileset\n";
            return;
        }
        
        Tileset tiles;
        if ( ! tiles.load(newbase) )
        {
            std::cout << "Can't load the new_tiles\n";
            return;
        }
        
        ts.appendTilesFrom(tiles);
        
        ts.save(tsname);
        
        std::cout << "Maybe is ok, bye!\n";
    }
};

static ToolManagerAutoRegister<TSTool_append> tsappend("append", "Appends tiles to the tileset");
