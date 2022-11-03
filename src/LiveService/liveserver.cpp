#include "log.h"
#include "liveserver.h"

#define START_EVENT_ID (QEvent::Type)2000
#define CLOSE_EVENT_ID (QEvent::Type)2001

LiveServer::LiveServer(int argc, char *argv[]):
	QCoreApplication(argc, argv)
{
	/*
	 * Метод start() надо вызывать, когда работает exec().
	 * Поэтому "вызываем" start() таким способом:
	 */
	postEvent(this, new QEvent(START_EVENT_ID));
}

bool LiveServer::start()
{
	/*
	 * todo: берём из ком. строки параметры (подключение к БД, TCP-порт и пр.)
	 * QCommandLineParser parser;
	 * ...
	 */

	Log::message("Starting server...");

	// Инициализируем ресурсы:
	if (!m_storage.start(/* здесь могут передаваться параметры подключения к БД */) ||
		!m_broadcaster.start(5001 /* TCP-порт */))
	{
		Log::error("Server start failed");
		return 0;
	}

	// Перед выходом будет вызвана функция остановки сервера:
	connect(this, SIGNAL(aboutToQuit()), this, SLOT(shutdown()));

	// Запускаем рабочие потоки:
	auto runThread = [this](auto &&f)
	{
		std::thread t(f, this);
		m_threads.emplace_back(std::move(t));
	};

	runThread(&LiveServer::doRead);
	runThread(&LiveServer::doStore);

	// OK:
	Log::message("Server started");

	return 1;
}

void LiveServer::shutdown()
{
	// ставим флаг завершения работы для потоков:
	m_shouldStop = true;

	for(size_t i=0; i<m_threads.size(); i++)
		m_threads[i].join();

	// освобождаем ресурсы:
	m_broadcaster.shutdown();
	m_storage.shutdown();

	Log::message("Server was shut down");
}

void LiveServer::doRead()
{
	QByteArray z;
	while(isRunning())
	{
		std::string x;
		bool eof = !(std::cin >> x);

		if (eof)
		{
			// выход
			Log::trace("posting close event");
			postEvent(this, new QEvent(CLOSE_EVENT_ID));
			break;
		}

		z += " " + QByteArray::fromStdString(x);

		/*
		 * todo:
		 *
		 * Строго говоря, стрим JSON'ов в общем виде надо парсить.
		 * Это не такая простая задача. В linux есть утилита - jq.
		 * Её можно поставить после драйвера для обработки его
		 * выходного потока - как один из вариантов. Или подробней
		 * изучить возможности Qt. Пока делаем просто:
		 */

		QJsonParseError e;
		QJsonDocument d = QJsonDocument::fromJson(z, &e);
		if (d.isNull())
		{
			switch (e.error)
			{
				case QJsonParseError::UnterminatedArray:
				case QJsonParseError::UnterminatedObject:
				case QJsonParseError::UnterminatedString:
					// пришёл кусок Json'а
					continue;
			}

			Log::error("Json parse error: " + e.errorString() + "\"" + z + "\"");
			z.clear();
			continue;
		}

		z.clear();

		Point p = Point::fromJson(d.object());

		m_broadcaster.appendPoint(p); // очередь на "радиовещание" (трансляцию)
		m_storageFifo.put(p); // очередь на сохранение в БД
	}
}

void LiveServer::doStore()
{
	while(isRunning())
	{
		if (m_storageFifo.isEmpty())
			QThread::msleep(1);
		else
			m_storage.store(m_storageFifo.takeAll());
	}

	if (!m_storageFifo.isEmpty())
		m_storage.store(m_storageFifo.takeAll());
}

bool LiveServer::event(QEvent *e)
{
	if (e->type() == START_EVENT_ID)
	{
		start();
		return true;
	}

	if (e->type() == CLOSE_EVENT_ID)
	{
		quit();
		return true;
	}

	return QCoreApplication::event(e);
}
