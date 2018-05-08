#include <iostream>
#include <cstdlib>
#include <string>
#include <wiringPi.h>
#include <time.h>
#include <unistd.h>

#define TRIG 24
#define ECHO 23

const double SoundVelocity=0.017;
const unsigned int MaxEchoTime=25000;

class Sonar {
public:
	Sonar();
	~Sonar();
	bool Ping();
	unsigned int getDistance() { Ping(); return Distance; };
private:
	unsigned int uSec;
	unsigned int Distance;;
};

Sonar::Sonar()
{
	if (wiringPiSetupGpio()) return;
	pinMode(TRIG, OUTPUT);
	pinMode(ECHO, INPUT);
	return;
}

Sonar::~Sonar()
{
	return;
}

bool Sonar::Ping()
{
	// Ping!
	digitalWrite(TRIG, HIGH);
	usleep(10);
	digitalWrite(TRIG, LOW);
	// Wait pong...
	while (digitalRead(ECHO) == LOW) {
		;
	}
	// Pong!
	timespec starttime, endtime;
	clock_gettime(CLOCK_MONOTONIC, &starttime);
	while (digitalRead(ECHO) == HIGH) {
		;
	}
	clock_gettime(CLOCK_MONOTONIC, &endtime);
	uSec = (endtime.tv_sec - starttime.tv_sec)*1000000;
	uSec += endtime.tv_nsec/1000 - starttime.tv_nsec/1000;
	if (uSec < MaxEchoTime) {
		double RealDistance = uSec * SoundVelocity;
		int rInt = RealDistance; // 整数部だけ
		if ((RealDistance-rInt) > 0.5) {
			rInt++;
		}
		// cout << lexical_cast<string>(RealDistance) << std::endl;
		Distance = rInt;
		return true;
	} else {
		Distance = 0;
		return false;
	}
}

using std::string;
using std::cout;
using std::endl;
int main()
{
	Sonar s;
	while(1) {
		cout << "\nS: " << s.getDistance() << std::flush;
		usleep(500000);
	}
	return 0;
}
