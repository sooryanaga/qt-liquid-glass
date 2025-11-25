#ifndef QTLIQUIDGLASS_H
#define QTLIQUIDGLASS_H

#include <QWidget>
#include <QString>

namespace QtLiquidGlass {

enum class Material {
    Sidebar,
    Sheet,
    Hud,
    WindowBackground,
    Popover,
    Menu,
    FullscreenUI
};

struct Options {
    double cornerRadius = 0.0;
    QString tintColor = "";    // "#RRGGBB" or "#RRGGBBAA"
    bool opaque = false;
};

// CREATE a glass effect behind a QWidget.
// Returns id >= 0, or -1 on failure.
int addGlassEffect(QWidget* widget,
                   Material material = Material::Sidebar,
                   const Options& opts = {});

// Modify corner radius, tint, opacity, etc.
void configure(int id, const Options& opts);

// Modify custom int properties (variant, scrimState, subduedState)
void setIntProperty(int id, const QString& key, long long value);

// Modify custom string properties
void setStringProperty(int id, const QString& key, const QString& value);

// Remove and cleanup
void remove(int id);

} // namespace QtLiquidGlass

#endif // QTLIQUIDGLASS_H

