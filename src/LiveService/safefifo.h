#ifndef FIFO_H
#define FIFO_H

/*
 * Потоко-безопасная очередь.
 * Пока в простейшем виде (не lock-free)
 */
template <class T>
class SafeFifo
{
public:
	void put(const T &x)
	{
		QMutexLocker locker(&m_mutex);
		m_queue.enqueue(x);
	}

	T take()
	{
		QMutexLocker locker(&m_mutex);
		return m_queue.dequeue();
	}

	QQueue<T> takeAll()
	{
		QMutexLocker locker(&m_mutex);
		QQueue<T> q;
		q.swap(m_queue);
		return std::move(q);
	}

	bool isEmpty() const
	{
		QMutexLocker locker(&m_mutex);
		return m_queue.isEmpty();
	}

	int count() const
	{
		QMutexLocker locker(&m_mutex);
		return m_queue.count();
	}

protected:
	QQueue<T> m_queue;
	mutable QMutex m_mutex;
};

#endif // FIFO_H
