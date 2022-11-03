#include "log.h"

QMutex Log::m;

void Log::error(const QString &msg)
{
	QMutexLocker __(&m);

	std::cerr << "Error: " << msg.toLocal8Bit().constData() << std::endl;
}

void Log::message(const QString &msg)
{
	QMutexLocker __(&m);

	std::cout << "Message: " << msg.toLocal8Bit().constData() << std::endl;

//	QFile d("D:\\CPP_COMMON\\!Qt5.5ReleaseDll\\z.txt");
//	if (d.open(QIODevice::Append))
//	{
//		d.write(msg.toLocal8Bit() + "\r\n");
//		d.close();
	//	}
}

void Log::trace(const QString &msg)
{
//	QMutexLocker __(&m);

//	std::cout << "Trace: " << msg.toLocal8Bit().constData() << std::endl;
}
