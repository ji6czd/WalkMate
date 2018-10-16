#include <Wire.h>
#define BZ 9
#define SRF02 0x70

void wait()
{
	Wire.beginTransmission(SRF02);
	Wire.write(0);
	Wire.endTransmission(true);
	Wire.requestFrom(SRF02, 1);
	int len = Wire.available();
	Serial.println(len);
}


bool startRanging()
{
	Wire.beginTransmission(SRF02);
	Wire.write(0);
	Wire.write(0x51);
	int ret = Wire.endTransmission();
	if (ret) {
		Serial.println(ret);
		return false;
	}
	return true;
}

int getRange()
{
	int range=0;
	Wire.beginTransmission(SRF02);
	Wire.write(0x02);
	if (Wire.endTransmission()) return 0;
	Wire.requestFrom(SRF02, 2);
	if (Wire.available() >= 2) {
		range = Wire.read();
		range <<= 8;
		range |= Wire.read();
	}
	return range;
}

void beep(int freq)
{
	tone(BZ, freq);
	delay(30);
	noTone(BZ);
}

void setup()
{
	Wire.begin();
	Serial.begin(115200);
	pinMode(BZ, OUTPUT);
}

void loop()
{
	if (startRanging() == false) {
		Serial.println("Error");
	}
	else {
		delay(70);
		int range = getRange();
		Serial.println(range);
		int freq=range/30;
		if (freq>13) freq=13;
		else if (freq>10) freq=10;
		else if (freq>7) freq=7;
		else if (freq>4) freq=4;
		Serial.println(freq);
		freq=1500-(freq*90);
		beep(freq);
		delay(200);
	}
	return;
}
