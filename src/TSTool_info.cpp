
#include "TSTool.hpp"
#include "ToolManager.hpp"
#include "Tileset.hpp"

#include <iostream>
#include <SDL/SDL_video.h>

using std::string;

class TSTool_info : public TSTool
{
public:
    void run(const std::vector<std::string>& params)
    {
        if ( params.size() < 1 )
        {
            std::cout << "Use: info <tileset>\n";
            return;
        }
        
        Tileset ts;
        if ( ! ts.load(params[0]) )
        {
            return;
        }
        
        const SDL_Surface * image = ts.image();
        
        std::cout << "Tileset information for '" << params[0] << "':\n"
                  << "header: '" << ts.header() << "'\n"
                  << "version: " << ts.version() << "\n"
                  << "tiles: " << ts.tile_count() << "\n"
                  << "tile size: " << ts.tile_width() << "x" << ts.tile_height() << "\n";
        
        if ( image )
        {
            int tpr = image->w / ts.tile_width();
            int rows = image->h / ts.tile_height();
            
            std::cout << "image size: " << image->w << "x" << image->h << "\n"
                      << "image tiles: " << tpr << "x" << rows << "\n"
                      << "image paletted: " << (image->format->palette ? "yes" : "no") << "\n"
                      << "image bps: " << (int)image->format->BitsPerPixel << "\n";
        }
        else
        {
            std::cout << "image: no image\n";
        }
    }
};

static ToolManagerAutoRegister<TSTool_info> tsinfo("info", "Shows information about the tileset");
