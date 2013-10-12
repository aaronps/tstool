#include "Tileset.hpp"

#include <fstream>
#include <algorithm>
#include <basetyps.h>
#include <SDL/SDL_video.h>
#include "SDL_error.h"
#include "SDL_video.h"
#include "EndianStream.hpp"
#include "SDL_png.h"

#include "json.h"

Tileset::Tileset()
{
    _image = 0;
    clear();
}

Tileset::~Tileset()
{
    clear();
}

void Tileset::clear()
{
    if ( _image )
    {
        SDL_FreeSurface(_image);
        _image = 0;
    }
    _attributes.clear();
    _header.clear();
    _tile_count = 0;
    _tile_height = 0;
    _tile_width = 0;
    _version = 0;
}

bool Tileset::loadNetPanzer(const std::string& tlsname,
                            const std::string& actname,
                            const int tiles_per_row)
{
    clear();
    
    std::ifstream tls(tlsname.c_str(), std::ios::binary);
    iEndianStream ies(tls);
    
    std::string idh = ies.readFixString(64);
    int vers = ies.read16Lite();
    int xpix = ies.read16Lite();
    int ypix = ies.read16Lite();
    int ntiles = ies.read16Lite();

    char xxx[768];
    tls.read(xxx, 768);
    
    std::vector<Tileset::Attribute> atts;
    
    uint8_t tmpbuf[3];
    
    for ( int n = 0; n < ntiles; n++ )
    {
        // 0 = attribute
        // 1 = move value
        // 2 = color
        tls.read(reinterpret_cast<char*>(tmpbuf), 3);
        atts.push_back(Attribute(tmpbuf[1], tmpbuf[2]));
    }
    
    int picw = xpix * tiles_per_row;
    int pich = (ntiles / tiles_per_row);

    if ( ntiles % tiles_per_row )
    {
        pich++;
    }

    pich *= ypix;
    
    SDL_Surface * surf = SDL_CreateRGBSurface(SDL_SWSURFACE, picw, pich, 8, 0, 0, 0, 0);
    
    {
        SDL_Color palette[256];
        memset(palette, 0, sizeof(palette));
        std::ifstream pal(actname.c_str(), std::ios::binary);
        for ( int n = 0; n < 256; n++ )
        {
            palette[n].r = pal.get();
            palette[n].g = pal.get();
            palette[n].b = pal.get();
        }

        SDL_SetColors(surf, (SDL_Color*)&palette, 0, 256);
    }
    
    int dest_x = 0, dest_y = 0;
    for ( int n = 0; n < ntiles; n++ )
    {
        for ( int t = 0; t < ypix; t++ )
        {
            tls.read(&((char*)(surf->pixels))[((dest_y+t)*surf->pitch)+dest_x], xpix);
        }
        
        dest_x += xpix;
        if ( dest_x >= picw )
        {
            dest_x = 0;
            dest_y += ypix;
        }
    }
    
    this->_header = idh;
    this->_image = surf;
    this->_tile_count = ntiles;
    this->_tile_height = ypix;
    this->_tile_width = xpix;
    this->_version = vers;
    this->_attributes.swap(atts);
}

void Tileset::saveNetPanzer(const std::string& tlsname,
                            const std::string& actname)
{
    if ( _tile_count == 0 )
    {
        return;
    }
    
    if ( _image->format->BitsPerPixel != 8 )
    {
        std::cout << "Can't save, not a paletted image\n";
        return;
    }
    
    std::ofstream tls(tlsname.c_str(), std::ios::binary);
    oEndianStream oes(tls);
    
    oes.writeFixString(_header, 64);
    oes.write16Lite(_version);
    oes.write16Lite(_tile_width);
    oes.write16Lite(_tile_height);
    oes.write16Lite(_tile_count);
    
    
    char pbuf[768] = {0};
    for (int n = 0; n < _image->format->palette->ncolors; n++ )
    {
        pbuf[(n*3)]   = _image->format->palette->colors[n].r;
        pbuf[(n*3)+1] = _image->format->palette->colors[n].g;
        pbuf[(n*3)+2] = _image->format->palette->colors[n].b;
    }
    
    tls.write(pbuf,768);
    
    for ( int n = 0; n < _tile_count; n++ )
    {
        tls.put(0);
        tls.put(_attributes[n].move_value);
        tls.put(_attributes[n].average_color);
    }
    
    int xpos = 0;
    int ypos = 0;
    char * pixels = reinterpret_cast<char *>(_image->pixels);
    
    for ( int n = 0; n < _tile_count; n++ )
    {
        char * bufptr = &pixels[(ypos*_image->pitch) + xpos];
        for ( int lin=0; lin < _tile_height; lin++ )
        {
            tls.write(bufptr, _tile_width);
            bufptr += _image->pitch;
        }
        
        xpos += _tile_width;
        if ( xpos >= _image->w )
        {
            xpos = 0;
            ypos += _tile_height;
        }
    }
    
    tls.close();
    
    std::ofstream pfile(actname.c_str(), std::ios::binary);
    pfile.write(pbuf, 768);
    pfile.close();
    
}

void Tileset::save(const std::string& basename)
{
    Json::Value root(Json::objectValue);
    root["idheader"] = _header;
    root["tile_count"] = _tile_count;
    root["tile_width"] = _tile_width;
    root["tile_height"] = _tile_height;
    root["version"] = _version;
    
    Json::Value move(Json::arrayValue);
    for ( int n = 0; n < this->_tile_count; n++ )
    {
        move.append((int)this->_attributes[n].move_value);
    }
    
    root["move"] = move;
    
    Json::Value avgcolor(Json::arrayValue);
    for ( int n = 0; n < this->_tile_count; n++ )
    {
        avgcolor.append((int)this->_attributes[n].average_color);
    }
    
    root["color"] = avgcolor;
    
    Json::FastWriter writer;
    std::ofstream out( (basename+ ".json").c_str());
    out << writer.write(root);
    out.close();
    
    SDL_SavePNG(this->_image, (basename + ".png").c_str());
}

bool Tileset::load(const std::string& basename)
{
    std::ifstream tsfile((basename+".json").c_str());
    
    Json::Value root;
    Json::Reader reader;
    if ( ! reader.parse(tsfile, root, false) )
    {
        std::cout << "Load tileset error: "
                  << reader.getFormattedErrorMessages()
                  << "\n";
        return false;
    }
    
    tsfile.close();
    
    std::string header = root["idheader"].asString();
    int tile_count = root["tile_count"].asInt();
    int tile_width = root["tile_width"].asInt();
    int tile_height = root["tile_height"].asInt();
    int version = root["version"].asInt();
    
    Json::Value& jmove = root["move"];
    Json::Value& javgcolor = root["color"];
    
    if ( ! jmove.isArray() || (jmove.size() != tile_count) )
    {
        std::cout << "Load tileset error: not all the tiles has a 'move' value\n";
        return false;
    }
    
    if ( ! javgcolor.isArray() || (javgcolor.size() != tile_count) )
    {
        std::cout << "Load tileset error: not all the tiles has a 'color' value\n";
        return false;
    }
    
    std::vector<Attribute> atts;
    
    for ( int n = 0; n < tile_count; n++ )
    {
        atts.push_back(Tileset::Attribute(jmove[n].asInt(),
                                          javgcolor[n].asInt()));
    }
    
    SDL_Surface * surf = 0;
    
    if ( tile_count )
    {
        surf = SDL_LoadPNG((basename+".png").c_str());
        if ( ! surf )
        {
            std::cout << "Load tileset error: " << SDL_GetError() << "\n";
            return false;
        }
        
        int total_pixels = tile_width * tile_height * tile_count;
        int pic_pixels = surf->w * surf->h;
        if ( pic_pixels < total_pixels )
        {
            std::cout << "Load tileset error: picture is smaller than needed\n";
            SDL_FreeSurface(surf);
            return false;
        }
    }
    
    this->_attributes.swap(atts);
    this->_header = header;
    this->_tile_count = tile_count;
    this->_tile_height = tile_height;
    this->_tile_width = tile_width;
    this->_version = version;
    this->_image = surf;
    
    return true;
}

static SDL_Surface * createImageFromFormat( const int w,
                                            const int h,
                                            const SDL_PixelFormat * format )
{
    SDL_Surface * s = SDL_CreateRGBSurface( SDL_SWSURFACE,
                                            w, h,
                                            format->BitsPerPixel,
                                            format->Rmask,
                                            format->Gmask,
                                            format->Bmask,
                                            format->Amask);
    
    if ( format->palette )
    {
        SDL_SetColors(s, format->palette->colors, 0, format->palette->ncolors);
    }
    
    SDL_FillRect(s, 0, 0);
    
    return s;
}

void Tileset::blitTile( const unsigned index,
                        const int x,
                        const int y,
                        SDL_Surface* dest )
{
    if ( index >= _tile_count )
    {
        return;
    }
    
    SDL_Rect drect = {x, y, 0, 0};
    SDL_Rect tile_rect = { 0, 0, _tile_width, _tile_height};
    
    int tpr = _image->w / _tile_width;
    
    tile_rect.y = (index / tpr) * _tile_height;
    tile_rect.x = (index % tpr) * _tile_width;
    
    SDL_BlitSurface(_image, &tile_rect, dest, &drect);
}

void Tileset::deleteTiles(const unsigned index, const unsigned count)
{
    if ( index >= _tile_count )
    {
        return;
    }
    
    unsigned can_delete = _tile_count - index;
    
    unsigned to_delete = std::min(can_delete, count);
    
    std::cout << "deleting " << to_delete << " tiles\n";
    
    int old_tiles_per_row = _image->w / _tile_width;
    
    int new_tile_count = _tile_count - to_delete;
    int new_rows = new_tile_count / old_tiles_per_row;

    if ( new_tile_count % old_tiles_per_row )
    {
        new_rows++;
    }
    
    int new_pw = _image->w;
    int new_ph = new_rows * _tile_height;
    
    std::cout << "new size is " << new_pw << "x" << new_ph << "\n";
    
    SDL_Surface * surf = createImageFromFormat(new_pw, new_ph, _image->format);
    
    int dx = 0;
    int dy = 0;
    int ctile = 0;
    
    // copy before delete zone
    for ( ; ctile < index; ctile++ )
    {
        blitTile(ctile, dx, dy, surf);
        dx += _tile_width;
        if ( dx >= surf->w )
        {
            dx = 0;
            dy += _tile_height;
        }
    }
    
    // skip delete zone
    _attributes.erase(_attributes.begin()+ctile, _attributes.begin()+(ctile+to_delete));
    ctile += to_delete;
    
    // copy after delete zone
    for (; ctile < _tile_count; ctile++ )
    {
        blitTile(ctile, dx, dy, surf);
        dx += _tile_width;
        if ( dx >= surf->w )
        {
            dx = 0;
            dy += _tile_height;
        }
    }
    
    _tile_count -= to_delete;
    
    SDL_Surface *ss = _image;
    _image = surf;
    SDL_FreeSurface(ss);
    
}
