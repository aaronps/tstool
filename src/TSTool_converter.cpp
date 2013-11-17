
#include "TSTool.hpp"
#include "ToolManager.hpp"

#include <iostream>
#include <stdio.h>

#include "Tileset.hpp"

class TSTool_netpanzer2new : public TSTool
{
public:
    void run(const std::vector<std::string>& params)
    {
        if ( params.size() < 3 )
        {
            std::cout << "Use: netpanzer2new <tileset.tls> <palette.act> <destination_without_extension> [tiles_per_row=40]\n";
            return;
        }
        
        int tpr = params.size() > 3 ? atoi(params[3].c_str()) : 40;
        
        Tileset ts;
        ts.loadNetPanzer(params[0], params[1], tpr);
        
        ts.save(params[2]);
    }
};

class TSTool_new2netpanzer : public TSTool
{
public:
    void run(const std::vector<std::string>& params)
    {
        if ( params.size() < 3 )
        {
            std::cout << "Use: new2netpanzer <tileset> <out_tls.tls> <out_palette.act>\n";
            return;
        }
        
        Tileset ts;
        ts.load(params[0]);
        
        ts.saveNetPanzer(params[1], params[2]);
    }
};

static ToolManagerAutoRegister<TSTool_netpanzer2new> np2new("netpanzer2new", "Converts netpanzer tileset format to new tileset format");
static ToolManagerAutoRegister<TSTool_new2netpanzer> new2np("new2netpanzer", "Converts new tileset format to old netpanzer tileset format");
