#include "QtLiquidGlass/QtLiquidGlass.h"
#include "QtLiquidGlassCommon.h"
#include <QWidget>
#include <QColor>

namespace QtLiquidGlass {

int addGlassEffect(QWidget* widget, Material material, const Options& opts) {
#ifdef PLATFORM_OSX
    widget->setAttribute(Qt::WA_NativeWindow);
    widget->setAttribute(Qt::WA_TranslucentBackground);
    
    int id = AddGlassEffectView(reinterpret_cast<void*>(widget->winId()), opts.opaque);
    if (id >= 0) {
        configure(id, opts);
        // Map material
        long long variant = 0;
        long long mat = 0;
        // (Simplified mapping for history)
        if (material == Material::Sidebar) { variant = 0; mat = 7; }
        SetGlassViewVariant(id, variant);
        SetGlassViewMaterial(id, mat);
    }
    return id;
#else
    return -1;
#endif
}

void configure(int id, const Options& opts) {
#ifdef PLATFORM_OSX
    double r=0,g=0,b=0,a=0;
    if (!opts.tintColor.isEmpty()) {
        QColor c(opts.tintColor);
        r = c.redF(); g = c.greenF(); b = c.blueF(); a = c.alphaF();
    }
    ConfigureGlassView(id, opts.cornerRadius, r, g, b, a);
#endif
}

void setIntProperty(int id, const QString& key, long long value) {
#ifdef PLATFORM_OSX
    if (key == "variant") SetGlassViewVariant(id, value);
    else if (key == "material") SetGlassViewMaterial(id, value);
#endif
}

void setStringProperty(int id, const QString& key, const QString& value) {}
void remove(int id) { RemoveGlassEffectView(id); }

}
