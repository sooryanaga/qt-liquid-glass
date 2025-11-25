#include "QtLiquidGlassCommon.h"
#ifdef PLATFORM_OSX
#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>
#include <objc/runtime.h>
#include <objc/message.h>
#include <map>

static std::map<int, NSView *> g_glassViews;
static int g_nextViewId = 0;
static const void *kGlassEffectKey = &kGlassEffectKey;
static const void *kBackgroundViewKey = &kBackgroundViewKey;

#define RUN_ON_MAIN(block) if ([NSThread isMainThread]) { block(); } else { dispatch_sync(dispatch_get_main_queue(), block); }

extern "C" int AddGlassEffectView(void* nativeViewPtr, bool opaque) {
  __block int resultId = -1;
  RUN_ON_MAIN(^{
    NSView *container = reinterpret_cast<NSView *>(nativeViewPtr);
    if (!container) return;
    
    // Simple child injection (Old Strategy)
    NSView *glass = nil;
    Class glassCls = NSClassFromString(@"NSGlassEffectView");
    if (glassCls) {
        glass = [[glassCls alloc] initWithFrame:container.bounds];
        if (opaque) {
             NSBox* bg = [[NSBox alloc] initWithFrame:container.bounds];
             bg.boxType = NSBoxCustom;
             bg.fillColor = [NSColor windowBackgroundColor];
             [container addSubview:bg positioned:NSWindowBelow relativeTo:nil];
             objc_setAssociatedObject(container, kBackgroundViewKey, bg, OBJC_ASSOCIATION_RETAIN);
        }
    } else {
        NSVisualEffectView *vis = [[NSVisualEffectView alloc] initWithFrame:container.bounds];
        vis.material = NSVisualEffectMaterialUnderWindowBackground;
        glass = vis;
    }
    glass.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
    [container addSubview:glass positioned:NSWindowBelow relativeTo:nil];
    
    objc_setAssociatedObject(container, kGlassEffectKey, glass, OBJC_ASSOCIATION_RETAIN);
    int id = g_nextViewId++;
    g_glassViews[id] = glass;
    resultId = id;
  });
  return resultId;
}

extern "C" void ConfigureGlassView(int viewId, double cornerRadius, double r, double g, double b, double a) {
  RUN_ON_MAIN(^{
    NSView* glass = g_glassViews[viewId];
    if (!glass) return;
    glass.wantsLayer = YES;
    glass.layer.cornerRadius = cornerRadius;
    
    NSColor* c = [NSColor colorWithRed:r green:g blue:b alpha:a];
    if ([glass respondsToSelector:@selector(setTintColor:)]) {
        [(id)glass setTintColor:c];
    } else {
        glass.layer.backgroundColor = c.CGColor;
    }
  });
}

extern "C" void SetGlassViewVariant(int viewId, int variant) {
  RUN_ON_MAIN(^{
    NSView* glass = g_glassViews[viewId];
    SEL sel = sel_registerName("set_variant:");
    if ([glass respondsToSelector:sel]) {
      ((void (*)(id, SEL, long long))objc_msgSend)(glass, sel, (long long)variant);
    }
  });
}

extern "C" void SetGlassViewMaterial(int viewId, int material) {
  RUN_ON_MAIN(^{
    NSView* glass = g_glassViews[viewId];
    if ([glass isKindOfClass:[NSVisualEffectView class]]) {
        [(NSVisualEffectView*)glass setMaterial:(NSVisualEffectMaterial)material];
    }
  });
}

extern "C" void SetGlassViewScrim(int viewId, int scrim) {
    // implementation...
}

extern "C" void SetGlassViewSubdued(int viewId, int subdued) {
    // implementation...
}

extern "C" void RemoveGlassEffectView(int viewId) {
    // cleanup...
}
#endif
