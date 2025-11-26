#ifndef FADESTACK_H
#define FADESTACK_H

#include <QStackedWidget>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

class FadeStack : public QStackedWidget {
    Q_OBJECT
public:
    explicit FadeStack(QWidget *parent = nullptr);
    
    void fadeTo(int index);

signals:
    void setLastPage(int index);

private:
    void onFadeFinished();
    bool m_isAnimating = false;
};

#endif // FADESTACK_H

