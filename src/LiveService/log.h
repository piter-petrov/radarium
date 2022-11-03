#ifndef LOG_H
#define LOG_H

/*
 * "Схематичный" класс для логирования
 */
class Log
{
public:
	static void error(const QString & msg);
	static void message(const QString & msg);
	static void trace(const QString & msg);
protected:
	static QMutex m;
};

#endif // LOG_H
