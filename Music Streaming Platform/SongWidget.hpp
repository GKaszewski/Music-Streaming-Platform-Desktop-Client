#pragma once

#include <QWidget>
#include "ui_SongWidget.h"
#include "Song.h"
#include "AudioPlayer.h"

class SongWidget : public QWidget{
	Q_OBJECT

public:
	SongWidget(QWidget *parent = Q_NULLPTR, Song* song = nullptr, AudioPlayer * audioPlayer = nullptr);
	SongWidget(const SongWidget& widget);
	~SongWidget();

public slots:
	void addSong();

private:
	Ui::SongWidget ui{};
	QPushButton* addButton;
	QLabel* songInfo;
	Song song;
	AudioPlayer * audioPlayer;
};
