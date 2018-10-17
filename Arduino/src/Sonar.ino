#define BZ 9
#define BW 8
#define PW 7


static int count=0;
static int Cycle=10;

void beep(int range)
{
	int freq=0;
	if (range == 0) return;
	if (range > 500) freq=500;
	else if (range > 400) freq=750;
	else if (freq > 300) freq=1000;
	else if (freq > 200) freq=1250;
	else if (range > 1500) freq = 1500;
	else if (range > 100) freq = 1750;
	else if (range > 70) freq = 2000;
	else if (range > 40) freq > 2250;
	else freq = 2500;
	tone(BZ, freq);
	delay(30);
	noTone(BZ);
}

int Ranging()
{
	unsigned int time=0;
	unsigned long begin, end;
	int range=0;

	while (digitalRead(PW) == LOW) ;
	begin=micros();
	while(digitalRead(PW)) ;
	end=micros();
	if (begin > end) return 0;
	time=end-begin;
	range=time/57;
	return range;
}

void CalcCycle(int range)
{
	if (range>400) Cycle=10;
	else if (range > 300) Cycle=7;
	if (range > 200) Cycle=5;
	else Cycle=3;
}

void setup()
{
	Serial.begin(115200);
	pinMode(BZ, OUTPUT);
	pinMode(BW, OUTPUT);
	pinMode(PW, INPUT);
	digitalWrite(BW, HIGH);
}

void loop()
{
	int range = Ranging();
	CalcCycle(range);
	if (count==Cycle) {
		beep(range);
		count=0;
	} else {
		count++;
	}
}
