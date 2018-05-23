#include <iostream>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <cstdlib>
#include <string>
#include <wiringPi.h>
#include <softTone.h>
#include <time.h>
#include <unistd.h>

#define TRIG 23
#define ECHO 24
#define BZ 18

const double SoundVelocity=0.017;
const unsigned int MaxEchoTime=30000; // usec 510cm
const unsigned int MinEchoTime = 1000; // usec 17cm
const int iRange=300; // Sonar range
const int MperS=40; // measure per sec
using boost::thread;
class Sonar {
public:
	Sonar();
	~Sonar();
	bool Start();
	unsigned int Stop();
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

Sonar::Sonar()
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
		;
	}
	// Pong!
	clock_gettime(CLOCK_MONOTONIC, &starttime);
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
	return true;
}

bool Sonar::Start()
{
  thread th(&Sonar::Measure, this);
	SonarThread = move(th);
	SonarThread.detach();
  return true;
}

unsigned int Sonar::Stop()
{
	SonarThread.interrupt();
	std::cout << mCount << std::endl;
	return mCount;
}

void Sonar::Measure()
{
	int d;
	while(1) {
		if (Ping() == false) {
		}
		usleep(1000);
	}
	return;
}

using namespace std;
int main()
{
	Sonar s;
	s.Start();
	while(1) {
		unsigned int d = s.getDistance();
		if(d > 15) {
			unsigned int t = ((double)iRange/d)*200;
			//cout << d << "   \r" << flush;
			softToneWrite(BZ, t);
			usleep(80000);
			softToneWrite(BZ, 0);
		}
		usleep(20000);
	}
	return 0;
}
