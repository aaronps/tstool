/* 
 * File:   IMG_png.h
 * Author: admin
 *
 * Created on June 3, 2013, 1:46 PM
 */

#ifndef IMG_PNG_H
#define	IMG_PNG_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "SDL_rwops.h"
    
struct SDL_Surface;
    
int IMG_isPNG(SDL_RWops *src);
SDL_Surface *IMG_LoadPNG_RW(SDL_RWops *src);

#define IMG_LoadPNG(fname) IMG_LoadPNG_RW(SDL_RWFromFile(fname, "rb"))

#ifdef	__cplusplus
}
#endif

#endif	/* IMG_PNG_H */

