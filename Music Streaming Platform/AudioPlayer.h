#pragma once

#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QMediaPlaylist>
#include <QtNetwork/QNetworkAccessManager>
#include "ui_AudioPlayer.h"
#include "Utilities.h"
#include <queue>

class QNetworkReply;
class AudioPlayer : public QWidget {
	Q_OBJECT

public:
	AudioPlayer(QWidget *parent = Q_NULLPTR);
	~AudioPlayer();

	QMediaPlayer * getMediaPlayer() const;
	
	std::queue<Song> queue;
	void updateUI();

private:
	void setPlayButton() const;
	
public slots:
	void play() const;

private slots:
	void previous();
	void skip();
	void pause() const;
	void handleMediaStatus();
	void onPositionChanged(qint64 position);
	void onDurationChanged(qint64 duration);
	void onVolumeChanged(int value);
	void onTimeMoved(qint64 duration);
	void onStateChanged();
	void onFinished(QNetworkReply* reply);

private:
	Ui::AudioPlayer ui;
	bool isPlaying = false;
	QMediaPlayer* m_player;
	std::queue<Song> lastPlayedSongs;
	QPushButton* playButton;
	QPushButton* previousButton;
	QPushButton* skipButton;
	QLabel* cover, *title, *author;
	QNetworkAccessManager * netManager;
	Utilities utils;
};
