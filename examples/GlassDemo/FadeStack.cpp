#include "FadeStack.h"
#include <QEasingCurve>

FadeStack::FadeStack(QWidget *parent) : QStackedWidget(parent) {
    setAutoFillBackground(false);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::transparent);
    setPalette(pal);
    

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Ignored);
}

void FadeStack::fadeTo(int index) {
    if (index == currentIndex() || index < 0 || index >= count()) return;
    
    QWidget* nextPage = widget(index);
    
    QGraphicsOpacityEffect* effect = dynamic_cast<QGraphicsOpacityEffect*>(nextPage->graphicsEffect());
    if (!effect) {
        effect = new QGraphicsOpacityEffect(nextPage);
        nextPage->setGraphicsEffect(effect);
    }
    
    effect->setOpacity(0.0);
    
    setCurrentIndex(index);
    
    QPropertyAnimation* anim = new QPropertyAnimation(effect, "opacity");
    anim->setDuration(300);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::OutQuad);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}
