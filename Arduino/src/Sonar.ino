#include <thread>

using namespace std;

#define BZ 13
#define BW 14
#define PW 12

static int channel = 0;
static int resolution = 8;
static int count=0;
static int Cycle=10;
static volatile int pRange;


class soundOut {
public:
  void beep(int f, int m) {
    freq=f;
    mSec=m;
    thread th(beepFunc);
    th.detach();
  };
  void waitBeep(int f, int m) {
    freq=f;
    mSec=m;
    thread th(beepFunc);
    th.join();
  };
private:
  static int freq;
  static int mSec;
  static void beepFunc();
};

int soundOut::freq, soundOut::mSec;

void soundOut::beepFunc()
{
	ledcWriteTone(0, freq);
	ledcWrite(0, 255);
	delay(mSec);
	ledcWriteTone(0, 0);
}

soundOut sOut;

void announce(int range)
{
	int freq=0;
	if (range == 0) return;
	if (range <= 40) freq = 2500;
	else if (range <= 70) freq = 2250;
	else if (range <= 100) freq = 2000;
	else if (range <= 150) freq = 1750;
	else if (range <= 200) freq = 1500;
	else if (range <= 300) freq=1250;
	else if (range <= 400) freq=1000;
	else if (range <= 500) freq=750;
	else if (range > 500) freq=500;
	sOut.beep(freq, 80);
}

void rangeDelay(int range)
{
	int delayTime=0;
	if (range <= 100) delayTime=150;
	else if (range <= 200) delayTime=250;
	else if (range <= 300) delayTime=350;
	else if (range <= 400) delayTime=500;
	else if (range > 400) delayTime=750;
	delay(delayTime);
}

void Ranging()
{
	unsigned int time=0;
	unsigned long begin, end;
	int range=0;
	begin=micros();
	while(digitalRead(PW)) ;
	end=micros();
	if (begin > end) return;
	time=end-begin;
	range=time/57;
	if (range > 650) return;
	pRange = range;
}

void startup()
{
	Serial.println("WalkMate started");
	sOut.waitBeep(1000, 150);
	sOut.waitBeep(750, 150);
	sOut.waitBeep(1250, 150);
	sOut.waitBeep(1000, 250);
	delay(100);
}

void setup()
{
	Serial.begin(115200);
	ledcSetup(channel, 1000, resolution);
	ledcAttachPin(BZ, channel);
	pinMode(BW, OUTPUT);
	pinMode(PW, INPUT);
	digitalWrite(BW, HIGH);
	// Interupt setting
	attachInterrupt( PW, Ranging, RISING);
	// Announce bootup
	startup();
}

void loop()
{
 rangeDelay(pRange);
 announce(pRange);
}
