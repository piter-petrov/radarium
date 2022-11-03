#include "log.h"
#include "storage.h"

Storage::Storage()
{

}

bool Storage::start()
{
	// todo: соединение с БД

	m_timer.start();

	return 1;
}

void Storage::shutdown()
{
	flush(); // если пользователь вдруг не сделал
}

void Storage::store(QQueue<Point> &&z)
{
	// переносим данные в общую очередь:
	while(z.count())
		m_delayed += z.dequeue();

	write();
}

void Storage::write(bool force)
{
	/*
	 * В БД данные надо записывать не по одному инсерту, а блоками
	 * (для экономии ресурсов). Оптимальное число записей на вставку
	 * зависит от разных факторов и ищется эмпирически. Поэтому
	 * используем стратегию отложенной записи данных. Т.о. ждём,
	 * когда прибудет достаточное число записей или пройдёт достаточно
	 * много времени (m_timer).
	 *
	 * Флаг force - для "насильной" записи.
	 */

	static const int optimal_inserts_count = 50; // магическое число
	static const int timeout_ms = 1000; // тоже магия

	int n = m_delayed.count();

	if (force ||
		n >= optimal_inserts_count ||
		n && m_timer.hasExpired(timeout_ms))
	{
		/*
		 * Записываем данные в БД.
		 *
		 * todo: сохранение в БД не реализовано.
		 *       Просто выводим сообщение.
		 */

		while(m_delayed.count())
		{
			Point x = m_delayed.dequeue();
			Log::trace(QString("The point with id=\"%1\" saved to Db").arg(x.id()));
		}

		QThread::msleep(5); // задержка в рамках имитации доступа к БД

		m_timer.start();
	}
}
