#pragma once

#include <QtMultimedia/QMediaPlaylist>
#include <vector>
#include "ui_MusicStreamingPlatformApp.h"
#include "Utilities.h"
#include "AudioPlayer.h"
#include "json.hpp"
#include "SongWidget.h"
#include "KeyEnterReceiver.h"

using json = nlohmann::json;

class MusicStreamingPlatformApp : public QMainWindow {
	Q_OBJECT

public:
	MusicStreamingPlatformApp(QWidget *parent = Q_NULLPTR);
	~MusicStreamingPlatformApp();

private:
	void keyPressEvent(QKeyEvent* event, QString query);
	json getSongsData(const QString & data);
	std::vector<Song> createSongsFromData(json data);
	void addSongWidgetsToLayout();

public slots:
	void search();

private slots:
	void onGetRequest(QNetworkReply* reply);
	
private:
	Ui::MusicStreamingPlatformAppClass ui;
	QNetworkAccessManager netManager;
	QPushButton* searchButton;
	AudioPlayer* audioPlayer;
	QVBoxLayout* searchedSongsLayout;
	std::vector<Song> searchedSongs;
	std::vector<std::unique_ptr<SongWidget>> songWidgets;
	KeyEnterReceiver * enterKey;
};