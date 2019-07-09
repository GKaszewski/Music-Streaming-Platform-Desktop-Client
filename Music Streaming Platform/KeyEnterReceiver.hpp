#pragma once
#include <qobject.h>

class KeyEnterReceiver : public QObject {
	Q_OBJECT
protected:
	bool eventFilter(QObject * obj, QEvent * event);
};

