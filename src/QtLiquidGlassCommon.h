#ifndef QTLIQUIDGLASS_COMMON_H
#define QTLIQUIDGLASS_COMMON_H

#if defined(__APPLE__) && defined(__MACH__)
    #define PLATFORM_OSX
#endif

#ifdef PLATFORM_OSX
    extern "C" {
        int AddGlassEffectView(void* nativeViewPtr, bool opaque);
        void ConfigureGlassView(int viewId, double cornerRadius, double r, double g, double b, double a);
        
        void SetGlassViewVariant(int viewId, int variant);
        void SetGlassViewMaterial(int viewId, int material);
        void SetGlassViewScrim(int viewId, int scrim);
        void SetGlassViewSubdued(int viewId, int subdued);
        
        void RemoveGlassEffectView(int viewId);
    }
#endif

#endif // QTLIQUIDGLASS_COMMON_H
