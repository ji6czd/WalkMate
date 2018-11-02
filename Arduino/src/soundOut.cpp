#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Arduino.h"
#include "SoundOut.hpp"

#include <thread>

using std::thread;
int soundOut::freq, soundOut::mSec, soundOut::vol;
soundOut sOut;

void soundOut::beep(int f, int m)
{
  freq=f;
  mSec=m;
  thread th(beepFunc);
  th.detach();
}

void soundOut::waitBeep(int f, int m)
{
  freq=f;
  mSec=m;
  thread th(beepFunc);
  th.join();
}

void soundOut::beepFunc()
{
	ledcWriteTone(0, freq);
	ledcWrite(0, vol);
	delay(mSec);
	ledcWriteTone(0, 0);
}

void soundOut::begin()
{
	ledcSetup(0, 1000, 13);
	ledcAttachPin(25, 0);
	vol=1;
}
