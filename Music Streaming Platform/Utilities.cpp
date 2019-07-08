#include "Utilities.h"
#include <QJsonObject>

Utilities::Utilities(){
}

Utilities::~Utilities(){
}

Song Utilities::createFromJSON(json data) {
	Song newSong;
	newSong.name = QString::fromStdString(data["title"].get<std::string>());
	newSong.audioUrl = QString::fromStdString(data["songUrl"].get<std::string>());
	newSong.albumName = QString::fromStdString(data["albumName"].get<std::string>());
	newSong.coverUrl = QString::fromStdString(data["albumUrl"].get<std::string>());
	newSong.author = QString::fromStdString(data["author"].get<std::string>());
	return newSong;
}

QString Utilities::encodeUrl(QString stringToEncode) {
	stringToEncode = stringToEncode.toLower();
	QString encoded;

	for (auto c : stringToEncode)
		if (c == ' ')
			encoded += "%20";
		else
			encoded += c;

	return encoded;
}

