#ifndef QTLIQUIDGLASS_COMMON_H
#define QTLIQUIDGLASS_COMMON_H

#if defined(__APPLE__) && defined(__MACH__)
    #define PLATFORM_OSX
#endif

#ifdef PLATFORM_OSX
    extern "C" {
        int AddGlassEffectView(void* nativeViewPtr, bool opaque);
        void ConfigureGlassView(int viewId, double cornerRadius, const char* tintHex);
        void SetGlassViewIntProperty(int viewId, const char* key, long long value);
        void SetGlassViewStringProperty(int viewId, const char* key, const char* value);
        void RemoveGlassEffectView(int viewId);
    }
#endif

#endif // QTLIQUIDGLASS_COMMON_H
