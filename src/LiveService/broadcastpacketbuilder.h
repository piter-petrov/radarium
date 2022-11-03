#ifndef BROADCASTPACKET_H
#define BROADCASTPACKET_H

#include "point.h"

/*
 * Класс для построение "радиовещательных"
 * пакетов (JSON-документов)
 */
class BroadcastPacketBuilder
{
public:
	static QByteArray dataPacket(QQueue<Point> & q);
	static QByteArray messagePacket(const QString & text);

protected:
	static QJsonObject baseJson(const QString & type);
	static QByteArray bytes(const QJsonObject & obj);
};

#endif // BROADCASTPACKET_H
