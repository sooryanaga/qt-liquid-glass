#include "QtLiquidGlass/QtLiquidGlass.h"
#include "QtLiquidGlassCommon.h"

#include <QWidget>
#include <QWindow>
#include <QColor>

namespace QtLiquidGlass {

int addGlassEffect(QWidget* widget, Material material, const Options& opts) {
#ifdef PLATFORM_OSX
    if (!widget) return -1;
    
    // Ensure the widget has a native window handle
    if (!widget->testAttribute(Qt::WA_NativeWindow)) {
        widget->setAttribute(Qt::WA_NativeWindow);
    }
    
    // IMPORTANT: The widget needs to be translucent for the glass to show through
    widget->setAttribute(Qt::WA_TranslucentBackground);
    
    // Get the winId (NSView*)
    WId wid = widget->winId();
    void* nativeViewPtr = reinterpret_cast<void*>(wid);
    
    int id = AddGlassEffectView(nativeViewPtr, opts.opaque);
    
    if (id >= 0) {
        configure(id, opts);
        
        // We will assume the Material enum maps to the 'variant' integer property.
        
        long long variantVal = 0;
        long long materialVal = 0; // For NSVisualEffectMaterial fallback
        
        // NSVisualEffectMaterial mappings:
        // Titlebar = 3, Selection = 4, Menu = 5, Popover = 6, Sidebar = 7, HeaderView = 10, Sheet = 11, WindowBackground = 12, HUDWindow = 13, FullScreenUI = 15, ToolTip = 17, ContentBackground = 18, UnderWindowBackground = 21, UnderPageBackground = 22
        
        switch (material) {
            case Material::Sidebar: 
                variantVal = 0; 
                materialVal = 7; // NSVisualEffectMaterialSidebar
                break;
            case Material::Sheet:   
                variantVal = 1; 
                materialVal = 11; // NSVisualEffectMaterialSheet
                break;
            case Material::Hud:     
                variantVal = 2; 
                materialVal = 13; // NSVisualEffectMaterialHUDWindow
                break;
            case Material::WindowBackground: 
                variantVal = 3; 
                materialVal = 12; // NSVisualEffectMaterialWindowBackground
                break;
            case Material::Popover: 
                variantVal = 4; 
                materialVal = 6; // NSVisualEffectMaterialPopover
                break;
            case Material::Menu:    
                variantVal = 5; 
                materialVal = 5; // NSVisualEffectMaterialMenu
                break;
            case Material::FullscreenUI: 
                variantVal = 6; 
                materialVal = 15; // NSVisualEffectMaterialFullScreenUI
                break;
        }
        
        // Send BOTH variant (for private class) and material (for public fallback)
        SetGlassViewVariant(id, (int)variantVal);
        SetGlassViewMaterial(id, (int)materialVal);
    }
    
    return id;
#else
    Q_UNUSED(widget);
    Q_UNUSED(material);
    Q_UNUSED(opts);
    return -1;
#endif
}

void configure(int id, const Options& opts) {
#ifdef PLATFORM_OSX
    // Parse color using QColor to get robust rgba values
    double r=0, g=0, b=0, a=0;
    if (!opts.tintColor.isEmpty()) {
        QColor c(opts.tintColor);
        if (c.isValid()) {
            r = c.redF();
            g = c.greenF();
            b = c.blueF();
            a = c.alphaF();
        }
    }
    
    ConfigureGlassView(id, opts.cornerRadius, r, g, b, a);
#else
    Q_UNUSED(id);
    Q_UNUSED(opts);
#endif
}

void setIntProperty(int id, const QString& key, long long value) {
#ifdef PLATFORM_OSX
    // Map string keys to explicit setter functions
    if (key == "variant") {
        SetGlassViewVariant(id, (int)value);
    } else if (key == "material") {
        SetGlassViewMaterial(id, (int)value);
    } else if (key == "scrim" || key == "scrimState") {
        SetGlassViewScrim(id, (int)value);
    } else if (key == "subdued" || key == "subduedState") {
        SetGlassViewSubdued(id, (int)value);
    }
#else
    Q_UNUSED(id);
    Q_UNUSED(key);
    Q_UNUSED(value);
#endif
}

void setStringProperty(int id, const QString& key, const QString& value) {
    // Deprecated/Unused in new backend structure, but kept for API compatibility
#ifdef PLATFORM_OSX
    Q_UNUSED(id);
    Q_UNUSED(key);
    Q_UNUSED(value);
#else
    Q_UNUSED(id);
    Q_UNUSED(key);
    Q_UNUSED(value);
#endif
}

void remove(int id) {
#ifdef PLATFORM_OSX
    RemoveGlassEffectView(id);
#else
    Q_UNUSED(id);
#endif
}

} // namespace QtLiquidGlass
