#ifndef _FIND_BEACON_HPP_
#define _FIND_BEACON_HPP_
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Arduino.h"
#include <M5Stack.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <vector>
#include <string>

using namespace std;
class findBeacon {
public:
  void begin();
private:
  bool isBeacon(string data);
  bool Announcenewdevice(BLEAdvertisedDevice &dev);
  int DetectNewDevices(BLEScanResults &newDevices, BLEScanResults &oldDevices);
  void findDeviceTask(void *pvParameters);
};
#endif
