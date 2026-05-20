/**
 * Helpers SDL3 pour le moteur TnC (mestoph).
 * Remplace l'ancien shim SDL2→SDL3 (tnc_sdl2_compat.h).
 */
#ifndef TNC_SDL3_H
#define TNC_SDL3_H

#include <SDL3/SDL.h>

/** Couleur 0xAARRGGBB (convention T4C / mestoph). */
inline Uint32 TnC_MapArgb(SDL_Surface *surface, Uint32 argb) {
    if (!surface) {
        return argb;
    }
    const Uint8 a = static_cast<Uint8>((argb >> 24) & 0xFFu);
    const Uint8 r = static_cast<Uint8>((argb >> 16) & 0xFFu);
    const Uint8 g = static_cast<Uint8>((argb >> 8) & 0xFFu);
    const Uint8 b = static_cast<Uint8>(argb & 0xFFu);
    return SDL_MapSurfaceRGBA(surface, r, g, b, a);
}

inline SDL_Surface *TnC_CreateRgbaSurface(int w, int h) {
    return SDL_CreateSurface(w, h, SDL_PIXELFORMAT_RGBA32);
}

inline SDL_Surface *TnC_CreateIndexedSurfaceFrom(void *pixels, int w, int h, int pitch) {
    SDL_Surface *s = SDL_CreateSurfaceFrom(w, h, SDL_PIXELFORMAT_INDEX8, pixels, pitch);
    if (s) {
        SDL_CreateSurfacePalette(s);
    }
    return s;
}

inline bool TnC_FillArgb(SDL_Surface *dst, const SDL_Rect *rect, Uint32 argb) {
    return SDL_FillSurfaceRect(dst, rect, TnC_MapArgb(dst, argb));
}

inline bool TnC_SetPalette(SDL_Surface *surface, const SDL_Color *colors, int first, int count) {
    if (!surface || !colors) {
        return false;
    }
    SDL_Palette *pal = SDL_GetSurfacePalette(surface);
    if (!pal) {
        pal = SDL_CreateSurfacePalette(surface);
    }
    return pal && SDL_SetPaletteColors(pal, colors, first, count);
}

inline bool TnC_SetColorKeyIndex(SDL_Surface *surface, Uint8 index) {
    return SDL_SetSurfaceColorKey(surface, true, index);
}

inline Uint32 TnC_GetTicksMs() {
    return static_cast<Uint32>(SDL_GetTicks());
}

inline void TnC_SetSurfaceAlpha(SDL_Surface *surface, Uint8 alpha) {
    if (!surface) {
        return;
    }
    SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_BLEND);
    SDL_SetSurfaceAlphaMod(surface, alpha);
}

inline SDL_Surface *TnC_ConvertToRgba(SDL_Surface *src) {
    if (!src) {
        return nullptr;
    }
    return SDL_ConvertSurface(src, SDL_PIXELFORMAT_RGBA32);
}

/** Index8 + palette + colorkey → surface RGBA32 (blits fiables vers layers RGBA). */
inline SDL_Surface *TnC_BakeIndexedSprite(SDL_Surface *indexed) {
    if (!indexed) {
        return nullptr;
    }
    SDL_Surface *rgba = SDL_ConvertSurface(indexed, SDL_PIXELFORMAT_RGBA32);
    SDL_DestroySurface(indexed);
    return rgba;
}

#endif /* TNC_SDL3_H */
