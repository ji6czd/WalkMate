#include <iostream>
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
	softToneCreate(BZ);
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
	int d;
	while(1) {
		d = s.getDistance();
		if (d > 150) {
			softToneWrite(BZ, 1000);
			usleep(500000);
			softToneWrite(BZ, 0);
		}
		else if (d > 100) {
			softToneWrite(BZ, 1500);
			usleep(500000);
			softToneWrite(BZ, 0);
		}
		else if (d > 75) {
			softToneWrite(BZ, 1750);
			usleep(500000);
			softToneWrite(BZ, 0);
		}
		else if (d > 50) {
			softToneWrite(BZ, 2000);
			usleep(500000);
			softToneWrite(BZ, 0);
		}
		else if (d > 30) {
			softToneWrite(BZ, 2500);
			usleep(500000);
			softToneWrite(BZ, 0);
		}
		//cout << "\n" << d << std::flush;
	}
	return 0;
}
