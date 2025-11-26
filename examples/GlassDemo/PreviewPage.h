#ifndef PREVIEWPAGE_H
#define PREVIEWPAGE_H

#include <QWidget>
#include <QMouseEvent>
#include <QLabel>
#include <QPushButton>
#include "QtLiquidGlass/QtLiquidGlass.h"

class PreviewPage : public QWidget {
    Q_OBJECT
public:
    explicit PreviewPage(QWidget *parent = nullptr);
    void setMaterial(QtLiquidGlass::Material mat);

signals:
    void clicked();
    void materialChanged(QtLiquidGlass::Material mat);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    void setupUi();
    void cycleMaterial(int delta);

    QLabel* m_hint;
    QLabel* m_materialLabel;
    
    int m_materialIndex = 0;
    QStringList m_materials;
};

#endif // PREVIEWPAGE_H
