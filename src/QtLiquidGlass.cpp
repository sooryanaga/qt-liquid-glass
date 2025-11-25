#include "QtLiquidGlass/QtLiquidGlass.h"
#include "QtLiquidGlassCommon.h"
#include <QWidget>

namespace QtLiquidGlass {

int addGlassEffect(QWidget* widget, Material material, const Options& opts) {
#ifdef PLATFORM_OSX
    widget->setAttribute(Qt::WA_NativeWindow);
    widget->setAttribute(Qt::WA_TranslucentBackground);
    return AddGlassEffectView(reinterpret_cast<void*>(widget->winId()), opts.opaque);
#else
    return -1;
#endif
}

void configure(int id, const Options& opts) {
#ifdef PLATFORM_OSX
    ConfigureGlassView(id, opts.cornerRadius, opts.tintColor.toUtf8().constData());
#endif
}

void setIntProperty(int id, const QString& key, long long value) {
#ifdef PLATFORM_OSX
    SetGlassViewIntProperty(id, key.toUtf8().constData(), value);
#endif
}

void setStringProperty(int id, const QString& key, const QString& value) {
#ifdef PLATFORM_OSX
    SetGlassViewStringProperty(id, key.toUtf8().constData(), value.toUtf8().constData());
#endif
}

void remove(int id) {
#ifdef PLATFORM_OSX
    RemoveGlassEffectView(id);
#endif
}

}
