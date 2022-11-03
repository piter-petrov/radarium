#ifndef STORAGE_H
#define STORAGE_H

#include "point.h"

/*
 * Сохранение в БД.
 */
class Storage
{
public:
	Storage();

	bool start();
	void shutdown();
	void store(QQueue<Point> && z);
	void flush() { write(true); }

protected:
	QQueue<Point> m_delayed; // очередь на запись в БД
	QElapsedTimer m_timer; // для фиксации момента последней реальной записи в БД

	void write(bool force = false);
};

#endif // STORAGE_H
