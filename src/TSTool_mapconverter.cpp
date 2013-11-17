
#include "TSTool.hpp"
#include "ToolManager.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include "SDL.h"
#include "SDL_png.h"
#include <stdio.h>

static std::string readString( std::ifstream& file, const unsigned len)
{
    char buf[len+1];
    memset(buf, 0, sizeof(buf));
    file.read(buf, len);
    
    unsigned slen = strlen(buf);
    
    return std::string(buf, slen);
}

static uint16_t readShort( std::ifstream& file)
{
    uint8_t buf[2] = { 0 };
    memset(buf, 0, sizeof(buf));
    
    file.read(reinterpret_cast<char*>(&buf), 2);
    
    return buf[0] | (buf[1] << 8);
}


class TSTool_map2new : public TSTool
{
public:
    void run(const std::vector<std::string>& params)
    {
        if ( params.size() < 3 )
        {
            std::cout << "Use: map2new <map_name_base> <palette.act> <destination_without_extension>\n";
            return;
        }
        
        std::string namebase(params[0]);
        std::string actname(params[1]);
        std::string outname(params[2]);
        
        std::string datafilename = namebase + ".npm";
        std::string outpostsname = namebase + ".opt";
        std::string spawnsname = namebase + ".spn";
        
        std::string jsfilename = outname + ".json";
        std::string thfilename = outname + ".png";
        
        std::ifstream mfile(datafilename.c_str(), std::ios::in | std::ios::binary);
        if ( ! mfile.is_open() )
        {
            std::cout << "Error: Can't open map file '" << datafilename << "'\n";
            return;
        }
        
        std::string idheader    = readString(mfile, 64);
        uint16_t id             = readShort(mfile);
        std::string name        = readString(mfile, 256);
        std::string description = readString(mfile, 1024);
        uint16_t width          = readShort(mfile);
        uint16_t height         = readShort(mfile);
        std::string tileset     = readString(mfile, 256);
        uint16_t th_width       = readShort(mfile);
        uint16_t th_height      = readShort(mfile);
        
        const unsigned num_elements = width * height;
        
        uint16_t map_data[num_elements];
        uint8_t th_data[num_elements];
        
        for ( int n = 0; n < num_elements; n++ )
        {
            map_data[n] = readShort(mfile);
        }
        
        for ( int n = 0; n < num_elements; n++ )
        {
            th_data[n] = mfile.get();
        }
        
        std::ifstream optfile(outpostsname.c_str(), std::ios::in | std::ios::binary);
        if ( ! optfile.is_open() )
        {
            std::cout << "Error: Can't open outposts file '" << outpostsname << "'\n";
            return;
        }
        
        std::vector<std::string> outpost_names;
        std::vector<unsigned> outpost_x;
        std::vector<unsigned> outpost_y;
        
        std::string skipstr;
        std::string strval;
        unsigned unval_x, unval_y;
        
        unsigned optcount = 0;
        
        optfile >> skipstr >> optcount;
        for (unsigned n = 0; n < optcount; n++ )
        {
            optfile >> skipstr >> strval;
            outpost_names.push_back(strval);
            
            optfile >> skipstr >> unval_x >> unval_y;
            outpost_x.push_back(unval_x);
            outpost_y.push_back(unval_y);
        }
        
        std::ifstream spnfile(spawnsname.c_str(), std::ios::in | std::ios::binary);
        if ( ! spnfile.is_open() )
        {
            std::cout << "Error: Can't open spawns file '" << spawnsname << "'\n";
            return;
        }
        
        std::vector<unsigned> spawn_x;
        std::vector<unsigned> spawn_y;
        
        unsigned spncount = 0;
        
        spnfile >> skipstr >> spncount;
        for (unsigned n = 0; n < spncount; n++ )
        {
            spnfile >> skipstr >> unval_x >> unval_y;
            spawn_x.push_back(unval_x);
            spawn_y.push_back(unval_y);
        }
        
        SDL_Color palette[256];
        memset(palette, 0, sizeof(palette));
        std::ifstream pal(actname.c_str(), std::ios::binary);
        if ( ! pal.is_open() )
        {
            std::cout << "Error opening palette file: " << actname << "\n";
            return;
        }
        
        for ( int n = 0; n < 256; n++ )
        {
            palette[n].r = pal.get();
            palette[n].g = pal.get();
            palette[n].b = pal.get();
        }
        
        SDL_Surface * thsurf = SDL_CreateRGBSurfaceFrom(th_data, th_width, th_height, 8, th_width, 0, 0, 0, 0);
        SDL_SetColors(thsurf, (SDL_Color*)&palette, 0, 256);
        
        std::cout << "idheader: '" << idheader << "'\n"
                  << "id: " << id << "\n"
                  << "name: '" << name << "'\n"
                  << "description: '" << description << "'\n"
                  << "width: " << width << "\n"
                  << "height: " << height<< "\n"
                  << "tileset: '" << tileset << "'\n"
                  << "th_width: " << th_width << "\n"
                  << "th_height: " << th_height<< "\n";
        
        for ( unsigned n = 0; n < outpost_names.size(); n++ )
        {
            std::cout << "Outpost: '" << outpost_names[n] << "' @ " << outpost_x[n] << "," << outpost_y[n] << "\n";
        }
        
        for ( unsigned n = 0; n < spawn_x.size(); n++ )
        {
            std::cout << "Spawn @ " << spawn_x[n] << "," << spawn_y[n] << "\n";
        }
        
        SDL_SavePNG(thsurf, thfilename.c_str());
        
       
        FILE *f = fopen(jsfilename.c_str(), "w");
        if ( !f )
        {
            std::cout << "ERROR: Cannot open destination file " << jsfilename << "\n";
            return;
        }

        fprintf(f, "{\n\t\"idheader\": \"%s\",\n", idheader.c_str());
        fprintf(f, "\t\"format_version\": %u,\n", 0);
        fprintf(f, "\t\"name\": \"%s\",\n", name.c_str());
        fprintf(f, "\t\"description\": \"%s\",\n", description.c_str());
        fprintf(f, "\t\"size\": [ %u, %u ],\n", width, height);
        fprintf(f, "\t\"tileset\": \"%s\",\n", tileset.c_str());
        
         fprintf(f, "\t\"spawns\": [");
        for ( unsigned n = 0; n < spawn_x.size(); n++ )
        {
            if ( n ) fprintf(f, ",");
            fprintf(f, "\n\t\t{ \"location\": [ %u, %u ] }", spawn_x[n], spawn_y[n] );
        }
        fprintf(f,"\n\t],\n");
        
        if ( ! outpost_names.empty() )
        {
            fprintf(f, "\t\"outposts\": [");
            for ( unsigned n = 0; n < outpost_names.size(); n++ )
            {
                if ( n ) fprintf(f, ",");
                fprintf(f, "\n\t\t{ \"name\": \"%s\", \"location\": [ %u, %u ] }", outpost_names[n].c_str(), outpost_x[n], outpost_y[n] );
            }
            fprintf(f,"\n\t],\n");
        }
        else
        {
            fprintf(f, "\t\"outposts\": [],\n");
        }
        
        fprintf(f, "\t\"data\": [\n\t\t%u", map_data[0]);
        for ( int n = 1; n < num_elements; n++ )
        {
            if ( (n % width) == 0 )
            {
                fprintf(f, ",\n\t\t%u", map_data[n] );
            }
            else
            {
                fprintf(f, ",%u", map_data[n]);
            }
        }
        
        fprintf(f, "\n\t]");
        
        // end of it
        fprintf(f, "\n}");
        
        fclose(f);

    }
};

//class TSTool_new2map : public TSTool
//{
//public:
//    void run(const std::vector<std::string>& params)
//    {
//        if ( params.size() < 3 )
//        {
//            std::cout << "Use: new2map <tileset> <out_tls.tls> <out_palette.act>\n";
//            return;
//        }
//        
//        Tileset ts;
//        ts.load(params[0]);
//        
//        ts.saveNetPanzer(params[1], params[2]);
//    }
//};

static ToolManagerAutoRegister<TSTool_map2new> map2new("map2new", "Converts netpanzer map files to new format");
//static ToolManagerAutoRegister<TSTool_new2map> new2map("new2map", "Converts new map files to old format");
