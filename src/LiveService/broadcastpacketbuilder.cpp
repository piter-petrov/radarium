#include "broadcastpacketbuilder.h"

QByteArray BroadcastPacketBuilder::dataPacket(QQueue<Point> &q)
{
	QJsonArray a;
	while(q.count())
		a.append(q.dequeue().toJson());

	QJsonObject z = baseJson("data");
	z["points"] = a;

	return bytes(z);
}

QByteArray BroadcastPacketBuilder::messagePacket(const QString &text)
{
	QJsonObject z = baseJson("message");
	z["text"] = text;

	return bytes(z);
}

QJsonObject BroadcastPacketBuilder::baseJson(const QString &type)
{
	QJsonObject obj;

	obj["type"] = type;

	return QJsonObject(obj);
}

QByteArray BroadcastPacketBuilder::bytes(const QJsonObject &obj)
{
	return QJsonDocument(obj).toJson(QJsonDocument::Compact);
}
