#define SW 9
#define BZ 8

void setup()
{
	pinMode(SW, INPUT_PULLUP);
}

void loop()
{
	if (digitalRead(SW) == LOW) {
		tone(BZ, 700);
		while (digitalRead(SW) == LOW);
		delay(10);
	}
	else if (digitalRead(SW) == HIGH) {
		noTone(BZ);
		while (digitalRead(SW) == HIGH);
	}
	return;
}
