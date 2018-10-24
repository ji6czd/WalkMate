#ifndef _SONAR_HPP_
#define _SONAR_HPP_
#include <thread>

class Sonar
{
public:
	void init();
	void startRanging();
private:
	void Announce();
	void rangeDelay();
	static void Ranging();
	static volatile unsigned int curRange;
};

#endif
