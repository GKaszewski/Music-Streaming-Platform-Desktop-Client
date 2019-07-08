#include "SongWidget.h"
#include "Song.h"
#include "AudioPlayer.h"

SongWidget::SongWidget(QWidget *parent, Song* song, AudioPlayer * audioPlayer)
	: QWidget(parent){

	ui.setupUi(this);
	this->audioPlayer = audioPlayer;
	this->song = *song;
	songInfo = ui.nameAndAuthor;
	songInfo->setText(this->song.name + "\n" + this->song.author);
	addButton = ui.addButton;
	this->show();
	connect(addButton, SIGNAL(clicked()), this, SLOT(addSong()));
}

SongWidget::SongWidget(const SongWidget& widget) {

}

SongWidget::~SongWidget(){
}

void SongWidget::addSong() {
	qDebug() << "Passing data!";
	audioPlayer->queue.push(song);
	audioPlayer->updateUI();
}


