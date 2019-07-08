#include "Song.h"

Song::Song() {
	name = "";
	albumName = "";
	author = "";
	audioUrl = "";
	coverUrl = "";
}

Song::Song(QString name, QString audioUrl, QString albumName, QString albumUrl, QString author) {
	this->name = name;
	this->albumName = albumName;
	this->author = author;
	this->audioUrl = audioUrl;
	this->coverUrl = coverUrl;
}

