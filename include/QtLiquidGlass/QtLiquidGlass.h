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
    QString tintColor = "";    // "#RRGGBB"
    bool opaque = false;
};

// CREATE a glass effect behind a QWidget.
int addGlassEffect(QWidget* widget,
                   Material material = Material::Sidebar,
                   const Options& opts = {});

void configure(int id, const Options& opts);
void setIntProperty(int id, const QString& key, long long value);
void setStringProperty(int id, const QString& key, const QString& value);
void remove(int id);

}

#endif // QTLIQUIDGLASS_H
