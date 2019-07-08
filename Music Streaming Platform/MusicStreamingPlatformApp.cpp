#include "MusicStreamingPlatformApp.h"
#include "Song.h"

#include <QKeyEvent>
#include <QDebug>
#include <QtNetwork/QNetworkReply>
#include <QJsonObject>
#include <QClipboard>
#include <memory>
#include "SongWidget.h"


MusicStreamingPlatformApp::MusicStreamingPlatformApp(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	audioPlayer = new AudioPlayer(this);
	ui.controlPanel->addWidget(audioPlayer, 0, Qt::AlignCenter);

	searchButton = ui.searchButton;
	searchedSongsLayout = ui.searchLayout;

	enterKey = new KeyEnterReceiver;
	ui.searchTextBox->installEventFilter(enterKey);

	//Connect search button with Search method.
	connect(searchButton, SIGNAL(clicked()), this, SLOT(search()));
	connect(&netManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onGetRequest(QNetworkReply*)));
}

MusicStreamingPlatformApp::~MusicStreamingPlatformApp() {
	delete searchButton;
	delete audioPlayer;
}

void MusicStreamingPlatformApp::keyPressEvent(QKeyEvent* event, QString query) {
	if (ui.searchTextBox->hasFocus()){
		if ((event->key() == Qt::Key_Enter) || (event->key() == Qt::Key_Return)) {
			qDebug() << "Search!";
			search();
		}
	}
}

json MusicStreamingPlatformApp::getSongsData(const QString & data) {
	if (data.isEmpty())
		return {};

	json parsedData = json::parse(data.toStdString());
	return parsedData;
}

std::vector<Song> MusicStreamingPlatformApp::createSongsFromData(json data) {
	std::vector<Song> songs;
	for (const auto & element : data) {
		qDebug() << "Element size: " << element.size();
		auto song = Utilities::createFromJSON(element);
		if (song.name != nullptr)
			songs.push_back(song);
	}

	qDebug() << "Songs count: " << songs.size();
	return songs;
}

void MusicStreamingPlatformApp::addSongWidgetsToLayout() {
	for (auto & song : searchedSongs) {
		qDebug() << song.name;
		songWidgets.push_back(std::make_unique<SongWidget>(nullptr, &song, audioPlayer));
	}

	for (auto & element : songWidgets) {
		searchedSongsLayout->addWidget(element.get());
	}
}

void MusicStreamingPlatformApp::search() {
	auto urlString = QString(Utilities::encodeUrl("https://streaming-platform-service.herokuapp.com/song/search/" + ui.searchTextBox->text()));
	auto url = QUrl(urlString);	
	netManager.get(QNetworkRequest(url));
	ui.searchTextBox->setText("");
}

void MusicStreamingPlatformApp::onGetRequest(QNetworkReply* reply) {
	if (reply->error() != QNetworkReply::NoError)
		return;

	auto replyString = reply->readAll();
	if(replyString.isEmpty()) return;
	const auto data = getSongsData(replyString);

	if (data.empty())
		return;

	qDebug() << "Size: " << data.size();
	searchedSongs = createSongsFromData(data);
	if(searchedSongs.empty()) return;

	addSongWidgetsToLayout();
}
