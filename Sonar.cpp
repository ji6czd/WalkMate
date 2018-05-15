#include <iostream>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <cstdlib>
#include <string>
#include <wiringPi.h>
#include <softTone.h>
#include <time.h>
#include <unistd.h>

#define TRIG 24
#define ECHO 23
#define BZ 13

const double SoundVelocity=0.017;
const unsigned int MaxEchoTime=50000;
const unsigned int iRange=400;
using boost::thread;
class Sonar {
public:
	Sonar();
	~Sonar();
	bool Start();
	bool Stop();
	unsigned int getDistance() { return Distance; };
	int CalcBeepSleepTime()
	{ return Distance ? 1000000/(iRange/Distance) : 0; }; 
private:
	bool Ping();
	void Measure();
	unsigned int uSec;
	unsigned int Distance;;
	bool fault;
	thread SonarThread;
	thread TimerThread;
};

Sonar::Sonar()
{
	if (wiringPiSetupGpio()) return;
	softToneCreate(BZ);
	pinMode(TRIG, OUTPUT);
	pinMode(ECHO, INPUT);
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
	while (digitalRead(ECHO) == HIGH) {
		;
	}
	clock_gettime(CLOCK_MONOTONIC, &endtime);
	//std::cout << "I: " << pintime.tv_nsec << std::endl;
	//std::cout << "I: " << inittime.tv_nsec << std::endl;
	//std::cout << "S: " << starttime.tv_nsec << std::endl;
	//std::cout << "E: " << endtime.tv_nsec << std::endl;
	uSec = (endtime.tv_sec - starttime.tv_sec)*1000000;
	uSec += endtime.tv_nsec/1000 - starttime.tv_nsec/1000;
	if (uSec < MaxEchoTime) {
		double RealDistance = uSec * SoundVelocity;
		int rInt = RealDistance; // ����������
		if ((RealDistance-rInt) > 0.5) {
			rInt++;
		}
		// cout << lexical_cast<string>(RealDistance) << std::endl;
		Distance = rInt;
		fault = false;
		return true;
	} else {
		if (!Distance) Distance = 0;
		fault = true;
		return false;
	}
}

bool Sonar::Start()
{
  thread th(&Sonar::Measure, this);
	SonarThread = move(th);
	SonarThread.detach();
  return true;
}

bool Sonar::Stop()
{
	SonarThread.interrupt();
	return true;
}

void Sonar::Measure()
{
	int d;
	while(1) {
		Ping();
		usleep(100000);
	}
	return;
}

using namespace std;
int main()
{
	Sonar s;
	s.Start();
	while(1) {
 		cout << "D: " << s.getDistance() << endl;
		if (s.getDistance()) {
			softToneWrite(BZ, 2000);
		} else {
			softToneWrite(BZ, 1500);
		}
		usleep(50000);
		softToneWrite(BZ, 0);
		usleep(400000);
	}
	return 0;
}
