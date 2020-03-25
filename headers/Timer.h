#ifndef TIMER_H
#define TIMER_H

#include <thread>
#include <chrono>
#include <functional>


// For parallel code execution, delayed code execution, repeated code execution:
class Timer
{
	// needed to not block the main thread
	std::thread myThread;
	bool alive = false;
	long repeatMaxNum = -1L;
	long repeatCounter = -1L;	// how many times a function was on hold
	std::chrono::milliseconds interval = std::chrono::milliseconds(0);
	std::function<void(void)> funct = nullptr;

	void sleepAndRun()
	{
		std::this_thread::sleep_for(interval);
		if (alive)
			getFunction()();
	}

	// the function passed to the thread
	void ThreadFunc()
	{
		if (repeatMaxNum == INFINITY)
			while (alive)
				sleepAndRun();
		else
			while (repeatCounter--)
				sleepAndRun();
	}

public:
	static const long INFINITY = -1L;

	Timer() = default;
	Timer(const std::function<void(void)> &f) : funct(f) {}
	Timer(const std::function<void(void)> &f, const unsigned long &i, const long repeatCount = Timer::INFINITY)
		: funct(f), interval(std::chrono::milliseconds(i)), repeatMaxNum(repeatCount) {}
	
	void start(bool isAsync = true)
	{
		if (alive)
			return;
		alive = true;
		repeatCounter = repeatMaxNum;
		if (isAsync)
			myThread = std::thread(ThreadFunc, this);
		else	// block main thread
			this->ThreadFunc();
	}

	void stop()
	{
		alive = false;
		myThread.join();	// blocks current thread until THIS (myThread) finishes exec
	}

	void setFunction(const std::function<void(void)> &f)
	{
		funct = f;
	}

	bool isAlive() const { return alive; }

	void setRepeatMaxNum(const long r)
	{
		if (alive)
			return;
		repeatMaxNum = r;
	}

	long getRepeatCounter() const { return repeatCounter; }

	long getRepeatMaxNum() const { return repeatMaxNum; }

	void setInterval(const unsigned long &i)
	{
		if (alive)
			return;
		interval = std::chrono::milliseconds(i);
	}

	unsigned long getInterval() const { return interval.count(); }

	const std::function<void(void)> &getFunction() const
	{
		return funct;
	}
};
#endif /* TIMER_H */