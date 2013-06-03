/*
 * SDL_SavePNG -- libpng-based SDL_Surface writer.
 *
 * This code is free software, available under zlib/libpng license.
 * http://www.libpng.org/pub/png/src/libpng-LICENSE.txt
 * This file comes from: https://github.com/driedfruit/SDL_SavePNG
 */

#ifndef savepng_h_
#define savepng_h_

#include "SDL_video.h"

#ifdef	__cplusplus
extern "C" {
#endif
    
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
extern int SDL_SavePNG_RW(SDL_Surface *surface, SDL_RWops *rw, int freedst);

/*
 * Return new SDL_Surface with a format suitable for PNG output.
 */
extern SDL_Surface *SDL_PNGFormatAlpha(SDL_Surface *src);

#ifdef	__cplusplus
}
#endif

#endif
