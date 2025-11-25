#include "MainWindow.h"
#include "QtLiquidGlass/QtLiquidGlass.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QDebug>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Use FramelessWindowHint for a pure glass look.
    // Our backend now supports this via Content Swapping.
    // Qt::FramelessWindowHint -> Triggers "Content Swap" backend strategy (no superview).
    // (Standard Windows -> Triggers "Sibling Injection" backend strategy via NSThemeFrame).
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    
    setupUi();
    resize(600, 200);
    
    // Initialize glass effect
    updateGlass();
}

MainWindow::~MainWindow() {
    if (glassId >= 0) {
        QtLiquidGlass::remove(glassId);
    }
}

void MainWindow::setupUi() {
    // Ensure the window palette is transparent so we don't paint over the glass
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::transparent);
    setPalette(pal);
    setAutoFillBackground(false); // Let the glass show through

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setAutoFillBackground(false); // Important!
    // Also make central widget transparent
    QPalette cPal = centralWidget->palette();
    cPal.setColor(QPalette::Window, Qt::transparent);
    centralWidget->setPalette(cPal);
    
    setCentralWidget(centralWidget);
    
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 1. Sidebar (Removed - we will glass the whole window)
    // sidebar = new QWidget(this);
    // sidebar->setFixedWidth(250);
    // sidebar->setAttribute(Qt::WA_TranslucentBackground); 
    
    // QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebar);
    // QLabel *title = new QLabel("Glass Sidebar", sidebar);
    // title->setStyleSheet("font-size: 18px; font-weight: bold; color: white;");
    // sidebarLayout->addWidget(title);
    // sidebarLayout->addStretch();
    
    // 2. Content Area (Controls)
    QWidget *content = new QWidget(this);
    // // content->setStyleSheet("background-color: #F0F0F0; color: #333;"); // REMOVED solid background
    QVBoxLayout *contentLayout = new QVBoxLayout(content);
    contentLayout->setAlignment(Qt::AlignVCenter);
    
    QFormLayout *controls = new QFormLayout();
    
    radiusSlider = new QSlider(Qt::Horizontal);
    radiusSlider->setRange(0, 50);
    radiusSlider->setValue(10);
    connect(radiusSlider, &QSlider::valueChanged, this, &MainWindow::updateGlass);
    
    tintInput = new QLineEdit(""); // Default to no tint (clear)
    connect(tintInput, &QLineEdit::editingFinished, this, &MainWindow::updateGlass);
    
    opaqueCheck = new QCheckBox("Opaque Background");
    connect(opaqueCheck, &QCheckBox::toggled, this, &MainWindow::updateGlass);
    
    materialCombo = new QComboBox();
    // // Standard macOS Materials
    materialCombo->addItems({"Sidebar", "Sheet", "Hud", "WindowBackground", "Popover", "Menu", "FullscreenUI"});
    // connect(materialCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::updateGlass);

    controls->addRow("Corner Radius:", radiusSlider);
    // controls->addRow("Tint (Hex):", tintInput);
    // controls->addRow("", opaqueCheck);
    // controls->addRow("Material:", materialCombo);
    
    contentLayout->addLayout(controls);
    
    // // Advanced Property Setters
    // contentLayout->addSpacing(20);
    // contentLayout->addWidget(new QLabel("<b>Advanced Properties</b>"));
    
    // QHBoxLayout *propLayout = new QHBoxLayout();
    // propKeyInput = new QLineEdit();
    // propKeyInput->setPlaceholderText("Key (e.g. variant)");
    // propValInput = new QLineEdit();
    // propValInput->setPlaceholderText("Value (int)");
    
    // setIntBtn = new QPushButton("Set Int");
    // connect(setIntBtn, &QPushButton::clicked, [this]() {
    //     if (glassId >= 0) {
    //         QtLiquidGlass::setIntProperty(glassId, propKeyInput->text(), propValInput->text().toLongLong());
    //     }
    // });
    
    // propLayout->addWidget(propKeyInput);
    // propLayout->addWidget(propValInput);
    // propLayout->addWidget(setIntBtn);
    
    // contentLayout->addLayout(propLayout);
    // contentLayout->addStretch();

    // mainLayout->addWidget(sidebar); // REMOVED
    mainLayout->addWidget(content);
}

void MainWindow::updateGlass() {
    QtLiquidGlass::Options opts;
    opts.cornerRadius = radiusSlider->value();
    opts.tintColor = tintInput->text();
    opts.opaque = opaqueCheck->isChecked();
    
    QtLiquidGlass::Material mat = static_cast<QtLiquidGlass::Material>(materialCombo->currentIndex());
    
    // Re-creating the glass effect is often safer when switching structural properties like 'opaque'
    // which might involve different view hierarchies (backing store NSBox vs direct).
    // However, 'configure' handles simple updates.
    
    if (glassId >= 0) {
        // If opaque state changed, we might want to remove and re-add, 
        // but for now let's try just configuring.
        // Actually, AddGlassEffectView implementation in .mm handles re-adding if called again 
        // on the same container because it checks for existing keys. 
        // So we can just call addGlassEffect again to update everything including structural changes.
        
        // But let's be explicit:
        QtLiquidGlass::configure(glassId, opts);
    } else {
        glassId = QtLiquidGlass::addGlassEffect(this, mat, opts);
    }
    
    // If material changed, we might need to re-apply it via addGlassEffect logic 
    // or explicit setIntProperty calls. 
    // Since addGlassEffect returns a new ID if it recreates the view, let's try that for full updates.
    
    // Force full re-creation to ensure all properties (including material mapping) apply correctly
    if (glassId >= 0) {
         // Note: Ideally we would remove the old one first if we want a clean slate,
         // but AddGlassEffectView handles replacement.
         glassId = QtLiquidGlass::addGlassEffect(this, mat, opts);
    }
}

void MainWindow::applyTint() {
    updateGlass();
}

void MainWindow::toggleEffect() {
    if (glassId >= 0) {
        QtLiquidGlass::remove(glassId);
        glassId = -1;
    } else {
        updateGlass();
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPosition().toPoint() - dragPosition);
        event->accept();
    }
}
