#ifndef LIVESERVER_H
#define LIVESERVER_H

#include "broadcaster.h"
#include "safefifo.h"
#include "point.h"
#include "storage.h"

/*
 * Самый главный класс в данном модуле.
 * См. метод start().
 *
 * Завершает работу при закрытии входного
 * потока (std::cin).
 */
class LiveServer: public QCoreApplication
{
	Q_OBJECT

public:
	LiveServer(int argc, char *argv[]);

private:
	SafeFifo<Point> m_storageFifo;
	Storage m_storage;
	Broadcaster m_broadcaster;
	volatile bool m_shouldStop{0}; // флаг, что надо завершать работу
	std::vector<std::thread> m_threads;

	inline bool isStopping() const { return m_shouldStop; }
	inline bool isRunning() const { return !m_shouldStop; }

	void doRead(); // обработка JSON-стрима от драйвера
	void doStore(); // сохранение в БД

	bool event(QEvent * e); // inherited

protected slots:
	void shutdown();
	bool start();
};

#endif // LIVESERVER_H
