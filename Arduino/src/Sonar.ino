#define BZ 9
#define BW 8
#define PW 7

#define MAX 3

static int count=0;

void beep(int range)
{
	int freq=range/30;
	if (freq>13) freq=13;
	else if (freq>10) freq=10;
	else if (freq>7) freq=7;
	else if (freq>4) freq=4;
	freq=1500-(freq*120);
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
	if (count==MAX) {
		beep(range);
		count=0;
	} else {
		count++;
	}
}
