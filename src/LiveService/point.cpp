#include "point.h"

/*
 * Видел в задаче, что 3 цифры на год, но я посчитал,
 * что это опечатка. QDateTime не поддерживает 3 цифры,
 * поэтому в учебной задаче переделывать не стану.
 */
#define DT_FMT "dd.MM.yyyy hh:mm:ss.zzz"

Point::Point(): m_x(qQNaN()), m_y(qQNaN())
{

}

QJsonObject Point::toJson() const
{
	QJsonObject obj;
	obj["id"] = m_id;
	obj["state"] = m_state;
	obj["ts"] = m_timestamp.toString(DT_FMT);

	if (hasPos())
	{
		QJsonObject pos;
		pos["x"] = m_x;
		pos["y"] = m_y;
		obj["pos"] = pos;
	}

	return QJsonObject(obj);
}

Point Point::fromJson(const QJsonObject & o)
{
	// todo: проверки на валидность JSON

	Point p;
	p.m_id = o["id"].toString();
	p.m_state = o["state"].toString();
	p.m_timestamp = QDateTime::fromString(o["ts"].toString(), DT_FMT);

	if (o.contains("pos"))
	{
		const QJsonObject &pos = o["pos"].toObject();
		p.m_x = pos["x"].toDouble();
		p.m_y = pos["y"].toDouble();
	}

	return p;
}
