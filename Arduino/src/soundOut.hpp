#ifndef _SOUNDOUT_HPP_
#define _SOUNDOUT_HPP_
#include <cstdint>

using namespace std;


class soundOut {
public:
  void begin();
	static void beep(int f, int m);
	static void waitBeep(int f, int m);
	static void morseOut(const char c);
private:
  const static int resolution = 8;
  static int freq;
  static int mSec;
  static int vol;
	static void beepFunc();
	static void morseLong();
	static void morseShort();
	static void morseSpace();
	static unsigned int cpm;
	static unsigned int tone;
};
extern soundOut sOut;

#endif
