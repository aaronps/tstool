/*
 * PNG read is copy from SDL_image
 * PNG save is copy from https://github.com/driedfruit/SDL_SavePNG
 */

#include "SDL_png.h"

#include <stdlib.h>
#include <stdio.h>

#include "SDL.h"
#include <png.h>

/* Check for the older version of libpng */
#if (PNG_LIBPNG_VER_MAJOR == 1) && (PNG_LIBPNG_VER_MINOR < 4)
#define LIBPNG_VERSION_12
#endif

/* Load a PNG type image from an SDL datasource */
static void png_read_data(png_structp ctx, png_bytep area, png_size_t size)
{
    SDL_RWops *src = (SDL_RWops *)png_get_io_ptr(ctx);
    SDL_RWread(src, area, size, 1);
}

SDL_Surface *SDL_LoadPNG_RW(SDL_RWops *src, int freesrc)
{
    int start;
    const char *error;
    SDL_Surface *volatile surface;
    png_structp png_ptr;
    png_infop info_ptr;
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_type, num_channels;
    Uint32 Rmask;
    Uint32 Gmask;
    Uint32 Bmask;
    Uint32 Amask;
    SDL_Palette *palette;
    png_bytep *volatile row_pointers;
    int row, i;
    volatile int ckey = -1;
    png_color_16 *transv;

    if ( !src ) {
            /* The error message has been set in SDL_RWFromFile */
            return NULL;
    }
    start = SDL_RWtell(src);

    /* Initialize the data we will clean up when we're done */
    error = NULL;
    png_ptr = NULL; info_ptr = NULL; row_pointers = NULL; surface = NULL;

    /* Create the PNG loading context structure */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                                      NULL,NULL,NULL);
    if (png_ptr == NULL){
            error = "Couldn't allocate memory for PNG file or incompatible PNG dll";
            goto done;
    }

     /* Allocate/initialize the memory for image information.  REQUIRED. */
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
            error = "Couldn't create image information for PNG file";
            goto done;
    }

    /* Set error handling if you are using setjmp/longjmp method (this is
     * the normal method of doing things with libpng).  REQUIRED unless you
     * set up your own error handlers in png_create_read_struct() earlier.
     */
#ifndef LIBPNG_VERSION_12
    if ( setjmp(*png_set_longjmp_fn(png_ptr, longjmp, sizeof (jmp_buf))) )
#else
    if ( setjmp(png_ptr->jmpbuf) )
#endif
    {
            error = "Error reading the PNG file.";
            goto done;
    }

    /* Set up the input control */
    png_set_read_fn(png_ptr, src, png_read_data);

    /* Read PNG header info */
    png_read_info(png_ptr, info_ptr);
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth,
                    &color_type, &interlace_type, NULL, NULL);

    /* tell libpng to strip 16 bit/color files down to 8 bits/color */
    png_set_strip_16(png_ptr) ;

    /* Extract multiple pixels with bit depths of 1, 2, and 4 from a single
     * byte into separate bytes (useful for paletted and grayscale images).
     */
    png_set_packing(png_ptr);

    /* scale greyscale values to the range 0..255 */
    if(color_type == PNG_COLOR_TYPE_GRAY)
            png_set_expand(png_ptr);

    /* For images with a single "transparent colour", set colour key;
       if more than one index has transparency, or if partially transparent
       entries exist, use full alpha channel */
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
            int num_trans;
            Uint8 *trans;
            png_get_tRNS(png_ptr, info_ptr, &trans, &num_trans,
                         &transv);
            if(color_type == PNG_COLOR_TYPE_PALETTE) {
                /* Check if all tRNS entries are opaque except one */
                int i, t = -1;
                for(i = 0; i < num_trans; i++)
                    if(trans[i] == 0) {
                        if(t >= 0)
                            break;
                        t = i;
                    } else if(trans[i] != 255)
                        break;
                if(i == num_trans) {
                    /* exactly one transparent index */
                    ckey = t;
                } else {
                    /* more than one transparent index, or translucency */
                    png_set_expand(png_ptr);
                }
            } else
                ckey = 0; /* actual value will be set later */
    }

    if ( color_type == PNG_COLOR_TYPE_GRAY_ALPHA )
            png_set_gray_to_rgb(png_ptr);

    png_read_update_info(png_ptr, info_ptr);

    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth,
                    &color_type, &interlace_type, NULL, NULL);

    /* Allocate the SDL surface to hold the image */
    Rmask = Gmask = Bmask = Amask = 0 ;
    num_channels = png_get_channels(png_ptr, info_ptr);
    if ( color_type != PNG_COLOR_TYPE_PALETTE ) {
            if ( SDL_BYTEORDER == SDL_LIL_ENDIAN ) {
                    Rmask = 0x000000FF;
                    Gmask = 0x0000FF00;
                    Bmask = 0x00FF0000;
                    Amask = (num_channels == 4) ? 0xFF000000 : 0;
            } else {
                    int s = (num_channels == 4) ? 0 : 8;
                    Rmask = 0xFF000000 >> s;
                    Gmask = 0x00FF0000 >> s;
                    Bmask = 0x0000FF00 >> s;
                    Amask = 0x000000FF >> s;
            }
    }
    surface = SDL_AllocSurface(SDL_SWSURFACE, width, height,
                    bit_depth*num_channels, Rmask,Gmask,Bmask,Amask);
    if ( surface == NULL ) {
            error = "Out of memory";
            goto done;
    }

    if(ckey != -1) {
            if(color_type != PNG_COLOR_TYPE_PALETTE)
                    /* FIXME: Should these be truncated or shifted down? */
                    ckey = SDL_MapRGB(surface->format,
                                     (Uint8)transv->red,
                                     (Uint8)transv->green,
                                     (Uint8)transv->blue);
            SDL_SetColorKey(surface, SDL_SRCCOLORKEY, ckey);
    }

    /* Create the array of pointers to image data */
    row_pointers = (png_bytep*) malloc(sizeof(png_bytep)*height);
    if ( (row_pointers == NULL) ) {
            error = "Out of memory";
            goto done;
    }
    for (row = 0; row < (int)height; row++) {
            row_pointers[row] = (png_bytep)
                            (Uint8 *)surface->pixels + row*surface->pitch;
    }

    /* Read the entire image in one go */
    png_read_image(png_ptr, row_pointers);

    /* and we're done!  (png_read_end() can be omitted if no processing of
     * post-IDAT text/time/etc. is desired)
     * In some cases it can't read PNG's created by some popular programs (ACDSEE),
     * we do not want to process comments, so we omit png_read_end

    lib.png_read_end(png_ptr, info_ptr);
    */

    /* Load the palette, if any */
    palette = surface->format->palette;
    if ( palette ) {
        int png_num_palette;
        png_colorp png_palette;
        png_get_PLTE(png_ptr, info_ptr, &png_palette, &png_num_palette);
        if(color_type == PNG_COLOR_TYPE_GRAY) {
            palette->ncolors = 256;
            for(i = 0; i < 256; i++) {
                palette->colors[i].r = i;
                palette->colors[i].g = i;
                palette->colors[i].b = i;
            }
        } else if (png_num_palette > 0 ) {
            palette->ncolors = png_num_palette; 
            for( i=0; i<png_num_palette; ++i ) {
                palette->colors[i].b = png_palette[i].blue;
                palette->colors[i].g = png_palette[i].green;
                palette->colors[i].r = png_palette[i].red;
            }
        }
    }

done:	/* Clean up and return */
    if ( png_ptr ) {
            png_destroy_read_struct(&png_ptr,
                                    info_ptr ? &info_ptr : (png_infopp)0,
                                                            (png_infopp)0);
    }
    if ( row_pointers ) {
            free(row_pointers);
    }
    if ( error ) {
            SDL_RWseek(src, start, RW_SEEK_SET);
            if ( surface ) {
                    SDL_FreeSurface(surface);
                    surface = NULL;
            }
            SDL_SetError(error);
    }

    if ( freesrc ) SDL_RWclose(src);

    return(surface); 
}

/* The following functions are from https://github.com/driedfruit/SDL_SavePNG */

#define SUCCESS 0
#define ERROR -1

#define USE_ROW_POINTERS

/* libpng callbacks */ 
static void png_error_SDL(png_structp ctx, png_const_charp str)
{
    SDL_SetError("libpng: %s\n", str);
}
static void png_write_SDL(png_structp png_ptr, png_bytep data, png_size_t length)
{
    SDL_RWops *rw = (SDL_RWops*)png_get_io_ptr(png_ptr);
    SDL_RWwrite(rw, data, sizeof(png_byte), length);
}

SDL_Surface *SDL_PNGFormatAlpha(SDL_Surface *src) 
{
    /* NO-OP for images < 32bpp and 32bpp images that already have Alpha channel */ 
    if (src->format->BitsPerPixel <= 24 || src->format->Amask) {
            src->refcount++;
            return src;
    }

    /* Convert 32bpp alpha-less image to 24bpp alpha-less image */
    SDL_Surface *surf = SDL_CreateRGBSurface(src->flags, src->w, src->h, 24,
            src->format->Rmask, src->format->Gmask, src->format->Bmask, 0);
    SDL_BlitSurface(src, NULL, surf, NULL);

    return surf;
}

int SDL_SavePNG_RW(SDL_Surface *surface, SDL_RWops *dst, int freedst) 
{
    png_structp png_ptr;
    png_infop info_ptr;
    png_colorp pal_ptr;
    SDL_Palette *pal;
    int i, colortype;
#ifdef USE_ROW_POINTERS
    png_bytep *row_pointers;
#endif
    /* Initialize and do basic error checking */
    if (!dst)
    {
            SDL_SetError("Argument 2 to SDL_SavePNG_RW can't be NULL, expecting SDL_RWops*\n");
            return (ERROR);
    }
    if (!surface)
    {
            SDL_SetError("Argument 1 to SDL_SavePNG_RW can't be NULL, expecting SDL_Surface*\n");
            if (freedst) SDL_RWclose(dst);
            return (ERROR);
    }
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, png_error_SDL, NULL); /* err_ptr, err_fn, warn_fn */
    if (!png_ptr) 
    {
            SDL_SetError("Unable to png_create_write_struct on %s\n", PNG_LIBPNG_VER_STRING);
            if (freedst) SDL_RWclose(dst);
            return (ERROR);
    }
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
            SDL_SetError("Unable to png_create_info_struct\n");
            png_destroy_write_struct(&png_ptr, NULL);
            if (freedst) SDL_RWclose(dst);
            return (ERROR);
    }
    if (setjmp(png_jmpbuf(png_ptr)))	/* All other errors, see also "png_error_SDL" */
    {
            png_destroy_write_struct(&png_ptr, &info_ptr);
            if (freedst) SDL_RWclose(dst);
            return (ERROR);
    }

    /* Setup our RWops writer */
    png_set_write_fn(png_ptr, dst, png_write_SDL, NULL); /* w_ptr, write_fn, flush_fn */

    /* Prepare chunks */
    colortype = PNG_COLOR_MASK_COLOR;
    if (surface->format->BytesPerPixel > 0
    &&  surface->format->BytesPerPixel <= 8
    && (pal = surface->format->palette))
    {
            colortype |= PNG_COLOR_MASK_PALETTE;
            pal_ptr = (png_colorp)malloc(pal->ncolors * sizeof(png_color));
            for (i = 0; i < pal->ncolors; i++) {
                    pal_ptr[i].red   = pal->colors[i].r;
                    pal_ptr[i].green = pal->colors[i].g;
                    pal_ptr[i].blue  = pal->colors[i].b;
            }
            png_set_PLTE(png_ptr, info_ptr, pal_ptr, pal->ncolors);
            free(pal_ptr);
    }
    else if (surface->format->BytesPerPixel > 3 || surface->format->Amask)
            colortype |= PNG_COLOR_MASK_ALPHA;

    png_set_IHDR(png_ptr, info_ptr, surface->w, surface->h, 8, colortype,
            PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

//	png_set_packing(png_ptr);
    png_set_bgr(png_ptr);

    /* Write everything */
    png_write_info(png_ptr, info_ptr);
#ifdef USE_ROW_POINTERS
    row_pointers = (png_bytep*) malloc(sizeof(png_bytep)*surface->h);
    for (i = 0; i < surface->h; i++)
            row_pointers[i] = (png_bytep)(Uint8*)surface->pixels + i * surface->pitch;
    png_write_image(png_ptr, row_pointers);
    free(row_pointers);
#else
    for (i = 0; i < surface->h; i++)
            png_write_row(png_ptr, (png_bytep)(Uint8*)surface->pixels + i * surface->pitch);
#endif
    png_write_end(png_ptr, info_ptr);

    /* Done */
    png_destroy_write_struct(&png_ptr, &info_ptr);
    if (freedst) SDL_RWclose(dst);
    return (SUCCESS);
}
