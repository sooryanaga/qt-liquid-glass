#include "PlayerPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QPainter>
#include <QPainterPath>
#include <QDebug>

PlayerPage::PlayerPage(QWidget *parent) : QWidget(parent) {
    songs.push_back({"daft_punk.jpg","Get Lucky", "Daft Punk"});
    songs.push_back({"pink_floyd.jpg", "Dark Side of the Moon","Pink Floyd"});
    songs.push_back({"tame_impala.jpg","Currents","Tame Impala"});
    songs.push_back({"beatles.jpg","Sgt. Pepper's Lonely Hearts Club Band","The Beatles"});
    songs.push_back({"white_stripes.jpg", "Elephant","The White Stripes"});
    songs.push_back({"tyler.jpg","Flower Boy","Tyler, The Creator"});

    setupUi();
}

void PlayerPage::next() {
    songIndex = songIndex == songs.size() - 1 ? 0 : songIndex + 1;
    setSong(songIndex);
}

void PlayerPage::prev() {
    songIndex = songIndex == 0 ? songs.size() - 1 : songIndex - 1;
    setSong(songIndex);
}

void PlayerPage::updateSongProgess() {
    int curValue = m_progressSlider->value();
    m_progressSlider->setValue(curValue==100 ? 0: curValue + 1);
}

static QPixmap roundedPixmap(const QPixmap &src, int radius)
{
    if (src.isNull()) return QPixmap();

    QPixmap dest(src.size());
    dest.fill(Qt::transparent);

    QPainter painter(&dest);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    QPainterPath path;
    path.addRoundedRect(src.rect(), radius, radius);

    painter.setClipPath(path);
    painter.drawPixmap(0, 0, src);

    return dest;
}

void PlayerPage::setSong(int index) {
    if (index < 0 || index >= songs.size()) {
        qWarning() << "Invalid song index:" << index;
        return;
    }

    Song *tempSong = &songs[index];

    QPixmap pixmap(":/covers/" + tempSong->coverAlias);

    if (!pixmap.isNull()) {
        QPixmap scaled = pixmap.scaled(
            100, 100,
            Qt::KeepAspectRatioByExpanding,
            Qt::SmoothTransformation
        );

        QPixmap rounded = roundedPixmap(scaled, 12);
        m_artLabel->setPixmap(rounded);
    } else {
        m_artLabel->setText("No Art");
    }

    m_titleLabel->setText(tempSong->songName);
    m_artistLabel->setText(tempSong->artistName);
}

void PlayerPage::setupUi() {
    m_progressTimer = new QTimer(this);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 0, 20, 20);
    mainLayout->setSpacing(0);

    QHBoxLayout *topBar = new QHBoxLayout();
    topBar->setSpacing(4);
    topBar->addStretch();

    auto createIconBtn = [](const QString& text) -> QPushButton* {
        QPushButton* btn = new QPushButton(text);
        btn->setCursor(Qt::PointingHandCursor);
        if (text.length() > 1) {
             btn->setFixedSize(60, 30);
             btn->setStyleSheet("QPushButton { color: rgba(255,255,255,0.8); background: rgba(255,255,255,0.1); border-radius: 5px; font-size: 12px; border: none; } QPushButton:hover { background: rgba(255,255,255,0.2); color: white; }");
        } else {
             btn->setFixedSize(30, 30);
             btn->setStyleSheet("QPushButton { color: rgba(255,255,255,0.6); background: transparent; border: none; font-size: 16px; } QPushButton:hover { color: white; }");
        }
        return btn;
    };

    QPushButton *resizeBtn = createIconBtn("Resize");
    connect(resizeBtn, &QPushButton::clicked, [this]() {
        m_isMini = !m_isMini;
        emit miniModeToggled(m_isMini);
    });
    topBar->addWidget(resizeBtn);

    QPushButton *blankBtn = createIconBtn("Preview");
    connect(blankBtn, &QPushButton::clicked, this, &PlayerPage::previewClicked);
    topBar->addWidget(blankBtn);

    QPushButton *settingsBtn = createIconBtn(u8"\u2699");
    connect(settingsBtn, &QPushButton::clicked, this, &PlayerPage::settingsClicked);
    topBar->addWidget(settingsBtn);
    
    mainLayout->addLayout(topBar);

    QHBoxLayout *contentArea = new QHBoxLayout();
    contentArea->setSpacing(20);

    m_artLabel = new QLabel(this);
    m_artLabel->setFixedSize(100, 100);
    m_artLabel->setStyleSheet("background-color: rgba(255,255,255,0.1); border-radius: 12px;");
    m_artLabel->setAlignment(Qt::AlignCenter);
    contentArea->addWidget(m_artLabel);

    QVBoxLayout *rightColumn = new QVBoxLayout();
    rightColumn->setSpacing(5);
    rightColumn->setContentsMargins(0, 5, 0, 5);

    m_titleLabel = new QLabel();
    m_titleLabel->setStyleSheet("font-size: 22px; font-weight: bold; color: white; background: transparent;");
    rightColumn->addWidget(m_titleLabel);
    
    m_artistLabel = new QLabel();
    m_artistLabel->setStyleSheet("font-size: 14px; color: rgba(255,255,255,0.7); background: transparent;");
    rightColumn->addWidget(m_artistLabel);
    
    rightColumn->addSpacing(10);

    QHBoxLayout *controlsRow = new QHBoxLayout();
    controlsRow->setSpacing(15);
    
    const QString btnStyle = "QPushButton { color: white; background: transparent; font-size: 24px; border: none; } QPushButton:hover { color: #00E5FF; }";
    QPushButton *prevBtn = new QPushButton(u8"\u23EE");
    m_playBtn = new QPushButton(u8"\u23EF");
    QPushButton *nextBtn = new QPushButton(u8"\u23ED");
    
    prevBtn->setStyleSheet(btnStyle);
    m_playBtn->setStyleSheet(btnStyle + " font-size: 32px;");
    nextBtn->setStyleSheet(btnStyle);
    
    connect(prevBtn, &QPushButton::clicked, this, &PlayerPage::prev);
    connect(nextBtn, &QPushButton::clicked, this, &PlayerPage::next);

    controlsRow->addWidget(prevBtn);
    controlsRow->addWidget(m_playBtn);
    controlsRow->addWidget(nextBtn);
    controlsRow->addStretch();

    rightColumn->addLayout(controlsRow);
    contentArea->addLayout(rightColumn);
    
    mainLayout->addLayout(contentArea);
    
    m_progressSlider = new QSlider(Qt::Horizontal, this);
    m_progressSlider->setValue(30);
    m_progressSlider->setStyleSheet(
        "QSlider::groove:horizontal { height: 4px; background: rgba(255,255,255,0.1); border-radius: 2px; }"
        "QSlider::handle:horizontal { background: white; width: 12px; height: 12px; margin: -4px 0; border-radius: 6px; }"
        "QSlider::sub-page:horizontal { background: #00E5FF; border-radius: 2px; }"
    );
    mainLayout->addWidget(m_progressSlider);

    setSong(songIndex);


}
