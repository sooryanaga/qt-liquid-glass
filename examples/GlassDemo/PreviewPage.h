#ifndef PREVIEWPAGE_H
#define PREVIEWPAGE_H

#include <QWidget>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QLabel>

class PreviewPage : public QWidget {
    Q_OBJECT
public:
    explicit PreviewPage(QWidget *parent = nullptr) : QWidget(parent) {
        m_hint = new QLabel("Click to exit preview", this);
        m_hint->setStyleSheet("color: rgba(255,255,255,0.3); font-size: 12px;");
        m_hint->move(20, 20);
    }

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *event) override {
        emit clicked();
        QWidget::mousePressEvent(event);
    }

private:
    QLabel* m_hint;
    QPropertyAnimation* m_labelFade = nullptr;
};

#endif // PREVIEWPAGE_H

