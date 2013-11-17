/* 
 * File:   Tileset.hpp
 * Author: admin
 *
 * Created on June 4, 2013, 2:31 PM
 */

#ifndef TILESET_HPP
#define	TILESET_HPP

#include "SDL_video.h"
#include <string>
#include <vector>
#include <stdint.h>

class Tileset
{
public:
    Tileset();
    Tileset(const std::string& header_text, const unsigned tile_w, const unsigned tile_h);
    ~Tileset();
    
    Tileset * subTileset(const std::string& header, const unsigned first, const unsigned count, const unsigned tiles_per_row) const;
    
    bool loadNetPanzer( const std::string& tlsname,
                        const std::string& actname,
                        const int tiles_per_row);
    
    void saveNetPanzer( const std::string& tlsname,
                        const std::string& actname);
    
    void save( const std::string& basename );
    bool load( const std::string& basename );
    
    const std::string& header() const { return _header; }
    unsigned version() const { return _version; }
    unsigned tile_count() const { return _tile_count; }
    unsigned tile_width() const { return _tile_width; }
    unsigned tile_height() const { return _tile_height; }
    const SDL_Surface * image() const { return _image; }
    
    uint8_t tile_move(const unsigned index) const
    {
        return index >= _tile_count ? 0xff : _attributes[index].move_value;
    }
    
    uint32_t tile_color(const unsigned index) const
    {
        return index >= _tile_count ? 0xffffffff : _attributes[index].average_color;
    }
    
    void deleteTiles(const unsigned index, const unsigned count);
    
    void blitTile(const unsigned index, const int x, const int y, SDL_Surface *dest) const;
    
    bool appendTilesFrom(const Tileset& other, const unsigned tiles_per_row = 40);
    
private:
    Tileset(const Tileset&);
    void operator=(const Tileset&);
    
    void clear();
    bool saveNiceJSon(const std::string& name) const;
    bool saveOnelinerJSon(const std::string& name) const;
    uint32_t calculateAverageTileColor(const unsigned index) const;
    
    struct Attribute
    {
        uint8_t move_value;
        uint32_t average_color;
        Attribute(uint8_t m, uint32_t c) : move_value(m), average_color(c) {}
    };

    std::string _header;
    unsigned _version;
    unsigned _tile_width;
    unsigned _tile_height;
    unsigned _tile_count;
    SDL_Surface * _image;

    std::vector<Attribute> _attributes;
};

#endif	/* TILESET_HPP */

