#ifndef PLAYERPAGE_H
#define PLAYERPAGE_H

#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QPushButton>
#include <QList>
#include <QTimer>

class PlayerPage : public QWidget {
    Q_OBJECT
public:
    explicit PlayerPage(QWidget *parent = nullptr);
    bool isMiniMode() const { return m_isMini; }

signals:
    void settingsClicked();
    void previewClicked();
    void miniModeToggled(bool mini);

private slots:
    void updateSongProgess();
private:
    void setupUi();
    void next();
    void prev();
    void setSong(int index);

    QLabel *m_artLabel;
    QLabel *m_titleLabel;
    QLabel *m_artistLabel;
    QSlider *m_progressSlider;
    QPushButton *m_playBtn;
    bool m_isMini = false;

    struct Song {
        QString coverAlias;
        QString songName;
        QString artistName;
    };
    QVector<Song> songs;
    int songIndex = 0;
    QTimer *m_progressTimer;
};

#endif // PLAYERPAGE_H

