#ifndef POINT_H
#define POINT_H

/*
 * Структура, соответствующая сущности
 * "состояние наблюдаемого объекта" в нашей системе.
 */
struct Point
{
	Point();

	QJsonObject toJson() const;
	static Point fromJson(const QJsonObject & o);

	QString id() const { return m_id; }

protected:
	QString m_id;
	double m_x; // предполагаем, что может быть пустое значение (NaN)
	double m_y; // предполагаем, что может быть пустое значение (NaN)
	QString m_state; // по-хорошем должен быть enum, конечно.
	QDateTime m_timestamp;

	bool hasPos() const { return qIsNaN(m_x); }
};

#endif // POINT_H
