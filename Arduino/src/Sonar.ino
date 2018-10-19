#define BZ 13
#define BW 14
#define PW 12

static int channel = 0;
static int resolution = 8;
static int count=0;
static int Cycle=10;

void espTone(int freq)
{
	ledcWriteTone(channel, freq);
	ledcWrite(channel, 10);
	delay(30);
	ledcWriteTone(channel, 0);
}

void beep(int range)
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
	espTone(freq);
}

void CalcCycle(int range)
{
	if (range <= 100) Cycle=3;
	else if (range <= 200) Cycle=5;
	else if (range <= 300) Cycle=7;
	else if (range <= 400) Cycle=10;
	else if (range > 400) Cycle=15;
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

void setup()
{
	Serial.begin(115200);
	ledcSetup(channel, 1000, resolution);
	ledcAttachPin(BZ, channel);
	pinMode(BW, OUTPUT);
	pinMode(PW, INPUT);
	digitalWrite(BW, HIGH);
}

void loop()
{
	int range = Ranging();
	CalcCycle(range);
	if (count>=Cycle) {
		beep(range);
		count=0;
	} else {
		count++;
	}
}

