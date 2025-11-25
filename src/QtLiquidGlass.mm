#include "QtLiquidGlassCommon.h"
#ifdef PLATFORM_OSX
#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>
#include <objc/runtime.h>
#include <objc/message.h>
#include <string>
#include <cctype>
#include <map>

static std::map<int, NSView *> g_glassViews;
static int g_nextViewId = 0;
static const void *kGlassEffectKey = &kGlassEffectKey;
static const void *kBackgroundViewKey = &kBackgroundViewKey;

static NSColor* ColorFromHexNSString(NSString* hex) {
  // Implementation of hex parsing
  NSString* cleaned = [[hex stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]] uppercaseString];
  if ([cleaned hasPrefix:@"#"]) cleaned = [cleaned substringFromIndex:1];
  if (cleaned.length != 6 && cleaned.length != 8) return nil;
  unsigned int rgba = 0;
  [[NSScanner scannerWithString:cleaned] scanHexInt:&rgba];
  CGFloat r = ((rgba & 0xFF0000) >> 16) / 255.0;
  CGFloat g = ((rgba & 0x00FF00) >> 8)  / 255.0;
  CGFloat b =  (rgba & 0x0000FF)        / 255.0;
  return [NSColor colorWithRed:r green:g blue:b alpha:1.0];
}

#define RUN_ON_MAIN(block) if ([NSThread isMainThread]) { block(); } else { dispatch_sync(dispatch_get_main_queue(), block); }

extern "C" int AddGlassEffectView(void* nativeViewPtr, bool opaque) {
  __block int resultId = -1;
  RUN_ON_MAIN(^{
    NSView *container = reinterpret_cast<NSView *>(nativeViewPtr);
    if (!container) return;
    
    NSView *glass = nil;
    Class glassCls = NSClassFromString(@"NSGlassEffectView");
    if (glassCls) {
        glass = [[glassCls alloc] initWithFrame:container.bounds];
    } else {
        NSVisualEffectView *vis = [[NSVisualEffectView alloc] initWithFrame:container.bounds];
        vis.material = NSVisualEffectMaterialUnderWindowBackground;
        glass = vis;
    }
    glass.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
    [container addSubview:glass positioned:NSWindowBelow relativeTo:nil];
    
    g_glassViews[g_nextViewId] = glass;
    resultId = g_nextViewId++;
  });
  return resultId;
}

extern "C" void ConfigureGlassView(int viewId, double cornerRadius, const char* tintHex) {
    // Configuration logic...
}

// Helper for dynamic setters
static SEL ResolveSetter(id obj, const char* cKey) {
    std::string key(cKey);
    std::string selName = "set" + key + ":"; // Simplified for brevity in this history step
    return sel_registerName(selName.c_str());
}

extern "C" void SetGlassViewIntProperty(int viewId, const char* key, long long value) {
    RUN_ON_MAIN(^{
        NSView* glass = g_glassViews[viewId];
        SEL sel = ResolveSetter(glass, key);
        if ([glass respondsToSelector:sel]) {
            ((void (*)(id, SEL, long long))objc_msgSend)(glass, sel, value);
        }
    });
}

extern "C" void SetGlassViewStringProperty(int viewId, const char* key, const char* value) {
    // Implementation...
}

extern "C" void RemoveGlassEffectView(int viewId) {
    // Cleanup...
}
#endif
