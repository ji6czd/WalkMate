#ifndef _SOUNDOUT_HPP_
#define _SOUNDOUT_HPP_

static volatile int pRange;

class soundOut {
public:
  void init(int pin, int channel);
  void beep(int f, int m);
  void waitBeep(int f, int m);
private:
  const static int resolution = 8;
  static int freq;
  static int mSec;
  static int vol;
  static void beepFunc();
};
#endif
