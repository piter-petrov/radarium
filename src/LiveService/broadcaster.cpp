#include "log.h"
#include "point.h"
#include "broadcastpacketbuilder.h"
#include "broadcaster.h"

#define NEW_DATA_EVENT_ID (QEvent::Type)2002

class NewDataEvent: public QEvent
{
public:
	NewDataEvent(): QEvent(NEW_DATA_EVENT_ID) { }
};

bool Broadcaster::start(int port)
{
	connect(&m_tcpServer, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
	m_tcpServer.listen(QHostAddress::Any, port);

	return 1;
}

void Broadcaster::shutdown()
{
	// больше не принимаем новых коннектов:
	m_tcpServer.close();

	QMutexLocker m(&m_mutex);

	// ...и закрываем старые:
	foreach(QTcpSocket* s, m_connections)
		s->close();

	foreach(QTcpSocket* s, m_connections)
		// проверяем, что ещё не закрыт - иначе выдаётся ошибка
		if (s->state() == QAbstractSocket::ClosingState)
			s->waitForDisconnected();
}

void Broadcaster::appendPoint(const Point &p)
{
	bool newData = m_fifo.isEmpty();

	m_fifo.put(p);

	/*
	 * Достаточно известить о том, что есть данные,
	 * лишь при появлении первого элемента. Затем - лишнее.
	 */
	if (newData)
		QCoreApplication::postEvent(this, new NewDataEvent());
}

int Broadcaster::sendData(QQueue<Point> &&q)
{
	return send(BroadcastPacketBuilder::dataPacket(q));
}

int Broadcaster::sendMessage(const QString &msg)
{
	/*
	 * По замыслу, иногда (сейчас или в будущем) могут
	 * быть ситуации, когда стоит известить клиента о чём-то:
	 * например, устройство "Радар" неожиданно выключилось.
	 * Ведь гораздо лучше, если пользователь сразу увидит у себя
	 * на экране причину прекращения трансляции.
	 */
	return send(BroadcastPacketBuilder::messagePacket(msg));
}

bool Broadcaster::event(QEvent *e)
{
	if (e->type() == NEW_DATA_EVENT_ID)
	{
		sendData(std::move(m_fifo.takeAll()));
		return true;
	}

	return QObject::event(e);
}

int Broadcaster::send(const QByteArray &z)
{
	QMutexLocker m(&m_mutex);

	foreach(QTcpSocket* s, m_connections)
		s->write(z);

	// ждём, когда все данные будут отправлены:
	foreach(QTcpSocket* s, m_connections)
		while(s->bytesToWrite())
		{
			s->waitForBytesWritten(1);

			if (s->state() != QAbstractSocket::ConnectedState)
				break;
		}

	return 1;
}

void Broadcaster::acceptConnection()
{
	QTcpSocket* s = m_tcpServer.nextPendingConnection();
	if (!s)
		return;

	connect(s, SIGNAL(error(QAbstractSocket::SocketError)),
			this, SLOT(handleError(QAbstractSocket::SocketError)));

	Log::message("New connection accepted");

	QMutexLocker m(&m_mutex);
	m_connections += s;
}

void Broadcaster::handleError(QAbstractSocket::SocketError socketError)
{
	if (socketError == QTcpSocket::RemoteHostClosedError)
	{
		/*
		 * Соединение закрыто, удаляем его из списка
		 */

		QMutexLocker m(&m_mutex);
		QTcpSocket* s = qobject_cast<QTcpSocket*>(sender());
		m_connections.removeOne(s);
	}
	else
		Log::error(QString("Socket error %1").arg(socketError));
}
