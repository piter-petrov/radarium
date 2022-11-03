#include <iostream>
#include <QDateTime>
#include <QThread>
#include <QElapsedTimer>

#define DT_FMT "dd.MM.yyyy hh:mm:ss.zzz"

QByteArray randomPoint(const QString & id, const QString & state)
{
	return QString("{\"id\":\"%1\",\"state\":\"%2\",\"pos\":{\"x\":%3,\"y\":%4},\"ts\":\"%5\"}")
			.arg(id)
			.arg(state)
			.arg(0.3)
			.arg(-0.4)
			.arg(QDateTime::currentDateTime().toString(DT_FMT))
			.toLatin1();
}

int main(int argc, char *argv[])
{
	/*
	 * Первый параметр командной строки - кол-во выдаваемых состояний в секунду
	 * Второй параметр - время работы в секундах.
	 */
	int countPerSecond = qMax((argc > 1)? QString(argv[1]).toInt(): 10, 1);
	int duration = qMax((argc > 2)? QString(argv[2]).toInt(): 10, 1);

	QElapsedTimer timer;
	timer.start();

	while(1)
	{
		std::cout << randomPoint("abc123", "leading").constData() << std::endl;
		QThread::msleep((int)(1000.0 / countPerSecond));

		if (timer.hasExpired(duration * 1000))
			break;
	}

	return 0;
}
