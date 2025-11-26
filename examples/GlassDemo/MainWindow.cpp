#include "MainWindow.h"
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // 1. Essential for the glass effect to show through the window
    //    Use FramelessWindowHint for a modern look.
    setWindowFlags(Qt::Window);
    
    // 2. Ensure the Qt window background is transparent
    setAutoFillBackground(false);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::transparent);
    setPalette(pal);

    // 3. Setup the actual content (Example is Music Player UI)
    setupUi();
    resize(620, 240);

    // 4. Apply the liquid glass effect
    QtLiquidGlass::Options opts;
    opts.cornerRadius = 16.0;
    opts.tintColor = "";    // Empty = clear glass
    opts.opaque = false;    // Allow desktop to show through
    
    // Create the effect behind our window
    m_glassId = QtLiquidGlass::addGlassEffect(this, QtLiquidGlass::Material::Sidebar, opts);
}

MainWindow::~MainWindow() {
    if (m_glassId >= 0) {
        QtLiquidGlass::remove(m_glassId);
    }
}

void MainWindow::updateGlass(QtLiquidGlass::Options opts, QtLiquidGlass::Material mat) {
    // Demonstration of updating settings at runtime
    if (m_glassId >= 0) {
        // Update parameters like radius, tint, opacity
        QtLiquidGlass::configure(m_glassId, opts);

        // Note: To change the MATERIAL (e.g. Sidebar -> Menu), we currently
        // re-add the effect. The library handles replacement automatically.
        m_glassId = QtLiquidGlass::addGlassEffect(this, mat, opts);
    }
}

void MainWindow::setupUi() {
    QWidget *central = new QWidget(this);
    
    // Important: Central widget must also be transparent
    central->setAutoFillBackground(false);
    QPalette pal = central->palette();
    pal.setColor(QPalette::Window, Qt::transparent);
    central->setPalette(pal);
    
    setCentralWidget(central);
    
    QVBoxLayout *layout = new QVBoxLayout(central);
    layout->setContentsMargins(0, 0, 0, 0);
    
    m_stack = new FadeStack(central);
    m_playerPage = new PlayerPage();
    m_settingsPage = new SettingsPage();
    m_previewPage = new PreviewPage();

    m_stack->addWidget(m_playerPage);
    m_stack->addWidget(m_settingsPage);
    m_stack->addWidget(m_previewPage);
    
    layout->addWidget(m_stack, 1);

    // Setup logic interactions
    setupConnections();
}

void MainWindow::setupConnections() {
    connect(m_stack, &FadeStack::setLastPage, [this](int index) {
        lastPageIndex = index;
    });

    connect(m_playerPage, &PlayerPage::settingsClicked, [this]() {
        m_stack->fadeTo(1); 
        if (height() < 400) animateResize(620, 400);
    });
    
    connect(m_playerPage, &PlayerPage::miniModeToggled, this, &MainWindow::toggleMiniMode);
    
    connect(m_playerPage, &PlayerPage::previewClicked, [this]() {
        m_stack->fadeTo(2); 
    });

    connect(m_settingsPage, &SettingsPage::backClicked, [this]() {
        m_stack->fadeTo(0); 
        
        if (m_playerPage->isMiniMode()) {
            animateResize(320, 240);
        } else {
            animateResize(620, 240);
        }
    });
    
    connect(m_settingsPage, &SettingsPage::glassSettingsChanged, this, &MainWindow::updateGlass);
    
    connect(m_settingsPage, &SettingsPage::previewMaterial, [this]() {
        m_stack->fadeTo(2); 
    });

    connect(m_previewPage, &PreviewPage::clicked, [this]() {
        m_stack->fadeTo(lastPageIndex); 
    });
}

void MainWindow::toggleMiniMode(bool mini) {
    if (mini) {
        animateResize(320, 240);
    } else {
        animateResize(620, 240);
    }
}

void MainWindow::animateResize(int w, int h) {
    if (width() == w && height() == h) return;
    
    QPropertyAnimation* anim = new QPropertyAnimation(this, "size");
    anim->setDuration(450);
    anim->setStartValue(size());
    anim->setEndValue(QSize(w, h));
    anim->setEasingCurve(QEasingCurve::OutExpo);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

// Window Dragging Logic (since we are frameless)

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPosition().toPoint() - m_dragPosition);
        event->accept();
    }
}
