/* 
 * File:   SDL_png.h
 * Author: admin
 *
 * Created on June 3, 2013, 3:53 PM
 */

#ifndef SDL_PNG_H
#define	SDL_PNG_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "SDL_rwops.h"
#include "SDL_video.h"
    
SDL_Surface *SDL_LoadPNG_RW(SDL_RWops *src, int freesrc);

#define SDL_LoadPNG(fname) SDL_LoadPNG_RW(SDL_RWFromFile(fname, "rb"), 1)

/* The following functions are from https://github.com/driedfruit/SDL_SavePNG */

/*
 * Save an SDL_Surface as a PNG file.
 *
 * Returns 0 success or -1 on failure, the error message is then retrievable
 * via SDL_GetError().
 */
#define SDL_SavePNG(surface, file) \
	SDL_SavePNG_RW(surface, SDL_RWFromFile(file, "wb"), 1)

/*
 * Save an SDL_Surface as a PNG file, using writable RWops.
 * 
 * surface - the SDL_Surface structure containing the image to be saved
 * dst - a data stream to save to
 * freedst - non-zero to close the stream after being written
 *
 * Returns 0 success or -1 on failure, the error message is then retrievable
 * via SDL_GetError().
 */
int SDL_SavePNG_RW(SDL_Surface *surface, SDL_RWops *rw, int freedst);

/*
 * Return new SDL_Surface with a format suitable for PNG output.
 */
SDL_Surface *SDL_PNGFormatAlpha(SDL_Surface *src);

#ifdef	__cplusplus
}
#endif

#endif	/* SDL_PNG_H */

