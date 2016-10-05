#include "../TimerCollections.hpp"
#include <iostream>
// Only for timing purposes
#include <thread>

int main(int argc, char** argv)
{
	TimerCollections<> DefaultTimerCheck;
	TimerCollections<defaultTimer> Timers;

	Timers.Add("Sleep For 1 second","1");
	Timers.Start();
	std::this_thread::sleep_for(std::chrono::seconds(1));
	Timers.Stop();
	Timers.Add("Sleep 4","2");
	Timers.Start();
	std::this_thread::sleep_for(std::chrono::seconds(1));
	Timers.Stop();
	Timers.Add("Sleep 4","These are just notes for the programmer\nThey support general strings. Like a commit message.");
	Timers.Add("Threaded Sleep 10");
	for(int i = 0; i < 4; ++i)
	{
		Timers.Add("Threaded Sleep N1");
	}
	Timers.Add("Threaded Sleep N1","Should be four of them");
	std::cout << Timers;

	return 0;
};
