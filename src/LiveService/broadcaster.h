#ifndef BROADCASTSERVER_H
#define BROADCASTSERVER_H

#include "safefifo.h"
#include "point.h"

/*
 * "Радиовещание" (трансляция) подписчикам по сети
 *
 * При создании соединения от клиента не требуется
 * никаких запросов, сервер автоматически начинает
 * рассылку с текущего момента. Для завершения
 * сеанса клиент просто должен закрыть соединение.
 */
class Broadcaster: public QObject
{
	Q_OBJECT

public:
	Broadcaster() {}

	bool start(int port);
	void shutdown();

	void appendPoint(const Point & p);

protected:
	SafeFifo<Point> m_fifo;
	QTcpServer m_tcpServer;
	QList<QTcpSocket*> m_connections; // активные соединения
	QMutex m_mutex; // доступ к списку активных соединений

	int send(const QByteArray & z);
	int sendData(QQueue<Point> &&q);
	int sendMessage(const QString & msg);

	bool event(QEvent * e); // inherited

protected slots:
	void acceptConnection();
	void handleError(QAbstractSocket::SocketError socketError);
};

#endif // BROADCASTSERVER_H
