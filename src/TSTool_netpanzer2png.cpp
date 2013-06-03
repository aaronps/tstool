
#include "TSTool.hpp"
#include "ToolManager.hpp"

#include <iostream>
#include <stdio.h>

#include "SDL.h"
#include "SDL_png.h"

struct TilesetHeader
{
    uint8_t	netp_id_header[64];
    uint16_t	version;
    uint16_t	x_pix;
    uint16_t	y_pix;
    uint16_t	tile_count;
    uint8_t	palette[768];
} __attribute__((packed));

struct TileAttStruct
{
    uint8_t att;
    uint8_t move;
    uint8_t color;
} __attribute__((packed));

static void readTile(SDL_Surface *dest, FILE * f)
{
    if ( SDL_MUSTLOCK(dest) ) SDL_LockSurface(dest);

    uint8_t * ptr = reinterpret_cast<uint8_t*>(dest->pixels);
    
    for ( int n = 0; n < dest->h; n++ )
    {
        fread(&ptr[n*dest->pitch], dest->w, 1, f);
    }
    
    if ( SDL_MUSTLOCK(dest) ) SDL_UnlockSurface(dest);
}

class TSTool_netpanzer2png : public TSTool
{
public:
    void run(const std::vector<std::string>& params)
    {
        if ( params.size() < 3 )
        {
            std::cout << "Use: netpanzer2png <tileset.tls> <palette.act> <destination_without_extension> [yes=remove last tile|*]\n";
            return;
        }
        
        int tiles_to_remove = 0;
        
        if ( params.size() > 3 )
        {
            if ( params[3] == "yes" )
            {
                tiles_to_remove = 1;
            }
        }
        
        FILE * f = fopen( params[0].c_str(), "rb");
        if ( !f )
        {
            printf("Can't open netpanzer tileset file %s", params[0].c_str());
            return;
        }

        TilesetHeader header;
        fread(&header, sizeof(header), 1, f);
        printf("Header ID: '%s'\n", header.netp_id_header);
        printf("Version: %d\n", header.version);
        printf("Tile Width: %d\n", header.x_pix);
        printf("Tile Height: %d\n", header.y_pix);
        printf("Num Tiles: %d\n", header.tile_count);

        FILE *palf = fopen(params[1].c_str(),"rb");
        if ( palf )
        {
            fread(header.palette,768,1,palf);
            fclose(palf);
        }
        else
        {
            printf("Error loading palette file '%s', using whatever is there", params[1].c_str());
        }
        
        TileAttStruct att;
        FILE * att_file = fopen( (params[2] + ".txt").c_str(), "wb");
        if ( att_file )
        {
            for ( int n = 0; n < header.tile_count; n++ )
            {
                fread(&att, sizeof(att), 1, f);
                if ( att.att != 0 )
                {
                    printf("There is actually something there!!!\n");
                }
                
                if ( n < (header.tile_count - tiles_to_remove) )
                {
                    fprintf(att_file, "%d,%d\n", att.move, att.color);
                }
            }

            fclose(att_file);
        }
        else
        {
            printf("Error saving attributes file");
        }
        
        header.tile_count -= tiles_to_remove;
    
        int picw = header.x_pix * 40; // xxx
        int pich = (header.tile_count / (picw/header.x_pix));

        if ( header.tile_count % (picw/header.x_pix) )
        {
            pich++;
        }

        pich *= header.y_pix;

//        printf("Size would be %dx%d\n", picw, pich);

        SDL_Color palette[256];
        memset(palette, 0, sizeof(palette));
        for ( int n = 0; n < 256; n++ )
        {
            palette[n].r = header.palette[(n*3)];
            palette[n].g = header.palette[(n*3)+1];
            palette[n].b = header.palette[(n*3)+2];
        }


        SDL_Surface * stile = SDL_CreateRGBSurface(SDL_SWSURFACE,header.x_pix, header.y_pix, 8,0,0,0,0);
        if ( ! SDL_SetColors(stile, (SDL_Color*)&palette, 0, 256) )
        {
            printf("Error setting palette");
        }

        SDL_Surface * surf = SDL_CreateRGBSurface(SDL_SWSURFACE, picw, pich, 8, 0, 0, 0, 0);
        SDL_SetColors(surf, (SDL_Color*)&palette, 0, 256);

        SDL_Rect r = { 0,0,0,0 };
        for ( int n = 0; n < header.tile_count; n++ )
        {
            readTile(stile, f);
            SDL_BlitSurface(stile, 0, surf, &r);

            r.x += header.x_pix;
            if ( r.x >= picw )
            {
                r.x = 0;
                r.y += header.y_pix;
            }
        }

        SDL_SavePNG(surf, (params[2] + ".png").c_str());

        SDL_FreeSurface(stile);
        SDL_FreeSurface(surf);

        fclose(f);
    }
};

static ToolManagerAutoRegister<TSTool_netpanzer2png> bmp2png("netpanzer2png", "Converts old netpanzer tileset to png format");
