#include <iostream>
#include <cstdlib>
#include <time.h>
const float SoundVelocity=0.017;
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
	if (uSec < MaxEchoTime) {
		Distance = uSec * SoundVelocity;
		return true;
	} else {
		Distance = 0;
		return false;
	}
}

int main()
{
	using std::cout;
	using std::endl;
	Sonar s;
	cout << s.getDistance() << endl;
	return 0;
}
