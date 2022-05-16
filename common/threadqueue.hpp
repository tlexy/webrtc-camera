#ifndef THREAD_QUEUE__H
#define THREAD_QUEUE__H

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>


template<typename T>
class ThreadQueue
{
public:
	ThreadQueue(int max = 10000)
		:_isNotEmpty(false),
		_isFull(false),
		_max(max)
	{
		if (_max < 0)
		{
			_max = 1;
		}
	}

	template<class _Rep,
		class _Period>
	bool push_back_t(const T& val, const std::chrono::duration<_Rep, _Period>& _Rel_time)
	{
		std::unique_lock<std::mutex> lock(_mutex);
		_cv.wait_for(lock, _Rel_time, [&] {return !_isFull; });
		if (_isFull)
		{
			return false;
		}
		_queue.push(val);
		if (_queue.size() >= _max)
		{
			_isFull = true;
		}
		_isNotEmpty = true;
		lock.unlock();
		_cv.notify_all();
		return true;
	}

	void push_back(const T& val)
	{
		std::unique_lock<std::mutex> lock(_mutex);
		_cv.wait(lock, [&] {return !_isFull; });
		_queue.push(val);
		if (_queue.size() >= _max)
		{
			_isFull = true;
		}
		_isNotEmpty = true;
		lock.unlock();
		_cv.notify_all();
	}

	/*void push_back(const T&& val)
	{
		push_back(std::move(val));
	}*/

	template<class _Rep,
		class _Period>
	T pop(bool& flag, const std::chrono::duration<_Rep, _Period>& sleep_time)
	{
		std::unique_lock<std::mutex> lock(_mutex);
		_cv.wait_for(lock, sleep_time, [&] {return _isNotEmpty; });
		if (!_isNotEmpty)
		{
			flag = false;
			return _default_val;
			/*T t;
			return t;*/
		}
		else
		{
			flag = true;
		}
		T val = _queue.front();
		_queue.pop();
		if(_queue.size() == 0)
		{
			_isNotEmpty = false;
		}
		/*if (_queue.size() >= _max)
		{
			_isFull = true;
		}*/
		if (_queue.size() < _max)
		{
			_isFull = false;
		}
		lock.unlock();
		if (_queue.size() < _max)
		{
			_cv.notify_all();
		}
		return val;
	}

	void clear()
	{
		std::lock_guard<std::mutex> lock_g(_mutex);

		while (!_queue.empty())
		{
			_queue.pop();
		}
		_isFull = false;
		_isNotEmpty = false;
	}

private:
	std::queue<T> _queue;
	std::condition_variable _cv;
	std::mutex _mutex;
	T _default_val{};
	int _max;
	bool _isFull;
	bool _isNotEmpty;
};

#endif