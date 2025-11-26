#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include <QWidget>
#include <QSlider>
#include <QLineEdit>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include "QtLiquidGlass/QtLiquidGlass.h"

class SettingsPage : public QWidget {
    Q_OBJECT
public:
    explicit SettingsPage(QWidget *parent = nullptr);

signals:
    void backClicked();
    void previewMaterial();
    void glassSettingsChanged(QtLiquidGlass::Options opts, QtLiquidGlass::Material mat);

private:
    void setupUi();
    void emitChange();
    void cycleMaterial(int delta);
    void updateCodeSnippet();
    void copyToClipboard();

    QSlider *radiusSlider;
    QLineEdit *tintInput;
    QCheckBox *opaqueCheck;
    
    QLabel *materialLabel;
    int m_materialIndex = 0;
    QStringList m_materials;

    QLineEdit *codeDisplay;
    QPushButton *copyBtn;
};

#endif // SETTINGSPAGE_H
