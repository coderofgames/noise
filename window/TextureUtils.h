#ifndef TEXTURE_UTILS_H
#define TEXTURE_UTILS_H

void SubtractTexture( Texture* result, Texture *source1, Texture *source2 );

UInt32 GetColor( LockableTexture *tex, int px, int py, int color );

/*
 * Format of CI colors is
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |    alpha      |         color index           |   fraction    |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

 #define CI_GETALPHA(ci)    ((ci) >> 24)

 #define CI_GETINDEX(ci)    (((ci) >> 8) & 0xffff)


 #define CI_GETFRACTION(ci) ((ci) & 0xff)


 #define CI_ROUNDINDEX(ci)  CI_GETINDEX((ci) + 0x80)


 #define CI_MASKALPHA(ci)   ((ci) & 0xffffff)


 #define CI_MAKE(a, i, f)    (((a) << 24) | ((i) << 8) | (f))


/*
 * Format of RGBA colors is
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |    alpha      |      red      |     green     |     blue      |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

 #define RGBA_GETALPHA(rgb)      ((rgb) >> 24)


 #define RGBA_GETRED(rgb)        (((rgb) >> 16) & 0xff)


 #define RGBA_GETGREEN(rgb)      (((rgb) >> 8) & 0xff)


 #define RGBA_GETBLUE(rgb)       ((rgb) & 0xff)


 #define RGBA_MAKE(r, g, b, a)   ((Int32) (((a) << 24) | ((r) << 16) | ((g) << 8) | (b)))


/* D3DRGB and D3DRGBA may be used as initialisers for D3DCOLORs
 * The float values must be in the range 0..1
 */

 #define D3DRGB(r, g, b) \
     (0xff000000L | (((long)((r) * 255)) << 16) | (((long)((g) * 255)) << 8) | (long)((b) * 255))

 #define D3DRGBA(r, g, b, a) \
     (  (((long)((a) * 255)) << 24) | (((long)((r) * 255)) << 16) \
     |   (((long)((g) * 255)) << 8) | (long)((b) * 255) \
    )


/*
 * Format of RGB colors is
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |    ignored    |      red      |     green     |     blue      |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

 #define RGB_GETRED(rgb)         (((rgb) >> 16) & 0xff)


 #define RGB_GETGREEN(rgb)       (((rgb) >> 8) & 0xff)


 #define RGB_GETBLUE(rgb)        ((rgb) & 0xff)


 #define RGBA_SETALPHA(rgba, x) (((x) << 24) | ((rgba) & 0x00ffffff))


 #define RGB_MAKE(r, g, b)       ((Int32) (((r) << 16) | ((g) << 8) | (b)))


 #define RGBA_TORGB(rgba)       ((Int32) ((rgba) & 0xffffff))


 #define RGB_TORGBA(rgb)        ((Int32) ((rgb) | 0xff000000))




#endif
