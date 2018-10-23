#include "soundOut.hpp"
#include "Sonar.hpp"
Sonar sn;
#define BZ 13

void startup()
{
	sOut.waitBeep(1000, 150);
	sOut.waitBeep(750, 150);
	sOut.waitBeep(1250, 150);
	sOut.waitBeep(1000, 250);
	delay(100);
	Serial.println("WalkMate started");
}

void setup()
{
	Serial.begin(115200);
	// Announce bootup
	sOut.init(BZ, 0);
	sn.init();
	Serial.println("Initializing sound output...");
	startup();
	sn.startRanging();
}

void loop()
{
}
