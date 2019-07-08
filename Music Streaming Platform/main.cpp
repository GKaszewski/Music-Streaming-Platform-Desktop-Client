#include "MusicStreamingPlatformApp.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MusicStreamingPlatformApp w;
	w.show();
	return a.exec();
}
