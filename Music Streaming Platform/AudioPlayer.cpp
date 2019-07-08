#include "AudioPlayer.h"
#include "Song.h"

#include <QDateTime>
#include <QNetworkReply>

void AudioPlayer::setPlayButton() const {
	if (isPlaying) {
		ui.play->setText("||");
		connect(ui.play, SIGNAL(clicked()), this, SLOT(pause()));
	}
	else {
		ui.play->setText("|>");
		connect(ui.play, SIGNAL(clicked()), this, SLOT(play()));
	}
}

AudioPlayer::AudioPlayer(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	
	title = ui.title;
	author = ui.author;
	cover = ui.cover;

	m_player = new QMediaPlayer(parent, QMediaPlayer::StreamPlayback);

	if (queue.empty()) ui.cover->setText((QString)"");

	setPlayButton();

	netManager = new QNetworkAccessManager(this);
	connect(netManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onFinished(QNetworkReply*)));

	connect(ui.previous, SIGNAL(clicked()), this, SLOT(previous()));
	connect(ui.skip, SIGNAL(clicked()), this, SLOT(skip()));

	connect(m_player, &QMediaPlayer::mediaStatusChanged, this, &AudioPlayer::handleMediaStatus);
	connect(m_player, &QMediaPlayer::stateChanged, this, &AudioPlayer::onStateChanged);
	connect(m_player, SIGNAL(positionChanged(qint64)), this, SLOT(onPositionChanged(qint64)));
	connect(m_player, &QMediaPlayer::durationChanged, this, &AudioPlayer::onDurationChanged);
	connect(m_player, SIGNAL(sliderMoved()), this, SLOT(onVolumeChanged(int)));
}

AudioPlayer::~AudioPlayer()
{
	delete m_player;
}

QMediaPlayer * AudioPlayer::getMediaPlayer() const {
	return m_player;
}

void AudioPlayer::onFinished(QNetworkReply* reply) {
	if (reply->error() != QNetworkReply::NoError) {
		qDebug() << "Error in " << reply->url() << " : " << reply->errorString();
		return;
	}

	auto data = reply->readAll();
	QPixmap pixmap;
	pixmap.loadFromData(data);
	ui.cover->setPixmap(pixmap.scaled(50, 50, Qt::KeepAspectRatio));
}

void AudioPlayer::updateUI() {
	if(!queue.empty()) {
		netManager->get(QNetworkRequest(QUrl(queue.front().coverUrl)));
		title->setText(queue.front().name);
		author->setText(queue.front().author);
	}
}


void AudioPlayer::previous() {
	if (!lastPlayedSongs.empty()) {
		m_player->setMedia(QUrl(lastPlayedSongs.front().audioUrl));
		netManager->get(QNetworkRequest(QUrl(lastPlayedSongs.front().coverUrl)));
		title->setText(lastPlayedSongs.front().name);
		author->setText(lastPlayedSongs.front().author);
		lastPlayedSongs.pop();
		play();
	}
}

void AudioPlayer::skip() {
	if(!queue.empty()) {
		lastPlayedSongs.push(queue.front());
		queue.pop();
		if (queue.empty()) return;

		m_player->setMedia(QUrl(queue.front().audioUrl));
		updateUI();
		play();
	}

	isPlaying = false;
	m_player->stop();
}

void AudioPlayer::pause() const {
	m_player->pause();
}

void AudioPlayer::play() const {
	if (queue.empty()) return;
	
	if(m_player->currentMedia().isNull()) {
		m_player->setAudioRole(QAudio::Role::MusicRole);
		m_player->setMedia(QUrl(queue.front().audioUrl));
	}

	m_player->play();
}

void AudioPlayer::handleMediaStatus() {
	/*if (m_player->mediaStatus() == QMediaPlayer::BufferingMedia || m_player->mediaStatus() == QMediaPlayer::BufferedMedia) 
		m_player->play();*/
}

void AudioPlayer::onPositionChanged(qint64 position) {
	ui.timeSlider->setValue(position);
	QDateTime currentTime;
	currentTime.setTime_t(position / 1000);
	ui.currentTime->setText(currentTime.toString("mm:ss"));
}

void AudioPlayer::onDurationChanged(qint64 duration) {
	ui.timeSlider->setMaximum(duration);
	QDateTime endTime;
	endTime.setTime_t(duration / 1000);
	ui.endTime->setText(endTime.toString("mm:ss"));
}

void AudioPlayer::onVolumeChanged(int value) {
	m_player->setVolume(value);
}

void AudioPlayer::onTimeMoved(qint64 duration) {
	
}

void AudioPlayer::onStateChanged() {
	if (m_player->state() == QMediaPlayer::PlayingState)
		isPlaying = true;
	else if (m_player->state() == QMediaPlayer::PausedState || m_player->state() == QMediaPlayer::StoppedState)
		isPlaying = false;
	else if (m_player->state() == QMediaPlayer::StoppedState) {
		skip();
		
	}

	if(lastPlayedSongs.size() >= 2) {
		lastPlayedSongs.pop();
	}

	setPlayButton();
}
