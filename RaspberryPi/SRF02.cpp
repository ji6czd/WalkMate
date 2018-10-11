#include <iostream>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <cstdlib>
#include <string>
#include <wiringPi.h>
#include <softTone.h>
#include <time.h>
#include <unistd.h>

#define BZ 18

class SRF02 {
public:
	SRF02();
	~SRF02();
	bool Start();
	 bool Stop();
	unsigned int getDistance() { return Distance; };
	int CalcBeepSleepTime()
	{ return Distance ? 1000000/(iRange/Distance) : 0; }; 
private:
	bool Ping();
	void Measure();
	unsigned int uSec; // echo time
	unsigned int Distance;
	bool fault; // pong not return
	thread SonarThread;
	thread TimerThread;
	unsigned int mCount;
};

Sonar::SRF02()
{
	if (wiringPiSetupGpio()) return;
	softToneCreate(BZ);
	pinMode(TRIG, OUTPUT);
	pinMode(ECHO, INPUT);
	mCount=0;
	return;
}

Sonar::~Sonar()
{
	Stop();
	return;
}

bool Sonar::Ping()
{
	// Ping!
	timespec pintime, starttime, endtime, inittime;
	clock_gettime(CLOCK_MONOTONIC, &pintime);
	digitalWrite(TRIG, HIGH);
	usleep(10);
	digitalWrite(TRIG, LOW);
	// Wait pong...
	clock_gettime(CLOCK_MONOTONIC, &inittime);
	while (digitalRead(ECHO) == LOW) {
		clock_gettime(CLOCK_MONOTONIC, &starttime);
		int u = (starttime.tv_sec - inittime.tv_sec)*1000000;
		u += (starttime.tv_nsec - inittime.tv_nsec)/1000;
		if (u > MaxEchoTime) {
			// Timeout
			Distance=0;
			fault=true;
			std::cout << "-" << std::flush;
			return false;
		}
	}
	// Pong!
	mCount++;
	while (digitalRead(ECHO) == HIGH) {
		timespec p; // passed time
		clock_gettime(CLOCK_MONOTONIC, &p);
		int u = (p.tv_sec - starttime.tv_sec)*1000000;
		u += (p.tv_nsec - starttime.tv_nsec)/1000;
		if (u > MaxEchoTime) {
			// Timeout
			Distance=0;
			fault=true;
			std::cout << "." << std::flush;
			return false;
		}
	}
	clock_gettime(CLOCK_MONOTONIC, &endtime);
	//std::cout << (endtime.tv_nsec - pintime.tv_nsec)/1000 << std::endl;
	//std::cout << "Is: " << pintime.tv_nsec << std::endl;
	//std::cout << "Ie: " << inittime.tv_nsec << std::endl;
	//std::cout << "S: " << starttime.tv_nsec << std::endl;
	//std::cout << "E: " << endtime.tv_nsec << std::endl;
	uSec = (endtime.tv_sec - starttime.tv_sec)*1000000;
	uSec += endtime.tv_nsec/1000 - starttime.tv_nsec/1000;
	//std::cout << uSec   << std::endl;
	if (uSec < MinEchoTime) {
		// Sensor error
		Distance=0;
		fault=true;
		std::cout << "m" << std::flush;
		return false;
	}
	double RealDistance = uSec * SoundVelocity;
	int rInt = RealDistance; // 整数部だけ
	if ((RealDistance-rInt) > 0.5) {
		rInt++;
	}
	// cout << lexical_cast<string>(RealDistance) << std::endl;
	Distance = rInt;
	fault = false;
	//std::cout << rint << std::endl;
	return true;
}

bool Sonar::Start()
{
	std::cout << "Start" << std::endl;
  thread th(&Sonar::Measure, this);
	SonarThread = move(th);
	SonarThread.detach();
  return true;
}

unsigned int Sonar::Stop()
{
	SonarThread.interrupt();
	std::cout << "Stop" << std::endl;
	return mCount;
}

void Sonar::Measure()
{
	int d;
	while(1) {
		if (Ping() == false) {
			// std::cout << "E" << std::endl;
		}
		delay(1000/MperS);
	}
	return;
}

using namespace std;
int main()
{
	Sonar s;
	s.Start();
	while(1) {
		unsigned int d=0;
		d = s.getDistance();
		if(d > 15) {
			softToneWrite(BZ, 1500);
			delay(30);
			softToneWrite(BZ, 0);
			delay(d*5);
		}
		delay(20);
	}
	return 0;
}
