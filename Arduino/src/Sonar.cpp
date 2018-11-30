#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Arduino.h"

#include "Sonar.hpp"
#include "soundOut.hpp"

#define PW 5
#define BW 8

volatile unsigned int Sonar::curRange;

void Sonar::begin()
{
	pinMode(BW, OUTPUT);
	pinMode(PW, INPUT);
	digitalWrite(BW, HIGH);
	// Interupt setting
	attachInterrupt( PW, Sonar::Ranging, RISING);
}

void Sonar::Announce()
{
 int freq=0;
	if (curRange == 0) return;
	if (curRange <= 40) freq = 2500;
	else if (curRange <= 70) freq = 2250;
	else if (curRange <= 100) freq = 2000;
	else if (curRange <= 150) freq = 1750;
	else if (curRange <= 200) freq = 1500;
	else if (curRange <= 300) freq=1250;
	else if (curRange <= 400) freq=1000;
	else if (curRange <= 500) freq=750;
	else if (curRange > 500) freq=500;
	sOut.beep(freq, 50);
}

void Sonar::rangeDelay()
{
	int delayTime=0;
	if (curRange <= 100) delayTime=150;
	else if (curRange <= 200) delayTime=250;
	else if (curRange <= 300) delayTime=350;
	else if (curRange <= 400) delayTime=500;
	else if (curRange > 400) delayTime=750;
	delay(delayTime);
}

void Sonar::Ranging()
{
	Serial.print(".");
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
	curRange = range;
}

void Sonar::startRanging()
{
	Serial.println("Start ranging...");
	std::thread th([this]() {for(;;) { Announce();	rangeDelay(); } });
	th.detach();
}
