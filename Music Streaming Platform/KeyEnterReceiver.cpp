#include "KeyEnterReceiver.h"
#include <QEvent>
#include <QDebug>
#include <QKeyEvent>

bool KeyEnterReceiver::eventFilter(QObject* obj, QEvent* event) {
	if (event->type() == QEvent::KeyPress) {
		QKeyEvent* key = static_cast<QKeyEvent*>(event);
		if ((key->key() == Qt::Key_Enter) || (key->key() == Qt::Key_Return)) {
			return true;
		}
		return QObject::eventFilter(obj, event);
	}
	return QObject::eventFilter(obj, event);

}
