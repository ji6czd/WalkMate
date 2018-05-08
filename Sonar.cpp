#include <iostream>
#include <cstdlib>
#include <string>
#include <time.h>

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
	uSec=25000;
	return;
}

Sonar::~Sonar()
{
	return;
}

bool Sonar::Ping()
{
	// Initial Sensor;
	// Ping!
	// Wait pong...
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
	cout << s.getDistance() << endl;
	return 0;
}
