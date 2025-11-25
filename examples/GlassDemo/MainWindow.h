#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSlider>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QComboBox>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateGlass();
    void applyTint();
    void toggleEffect();

private:
    QWidget *sidebar;
    int glassId = -1;

    QSlider *radiusSlider;
    QLineEdit *tintInput;
    QCheckBox *opaqueCheck;
    QComboBox *materialCombo;
    
    // Custom properties
    QLineEdit *propKeyInput;
    QLineEdit *propValInput;
    QPushButton *setIntBtn;
    QPushButton *setStrBtn;

    void setupUi();
    
    // Dragging support
    QPoint dragPosition;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // MAINWINDOW_H

