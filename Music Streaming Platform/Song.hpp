#pragma once

#include <QObject>

class Song{
public:
	QString name;
	QString albumName;
	QString audioUrl;
	QString coverUrl;
	QString author;

	Song();
	Song(QString name, QString audioUrl, QString albumName, QString albumUrl, QString author);
};
