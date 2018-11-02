#ifndef _FIND_BEACON_HPP_
#define _FIND_BEACON_HPP_
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Arduino.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <vector>
#include <string>

using namespace std;
class findBeacon {
public:
  static void begin();
private:
  static bool isBeacon(string data);
  static bool Announcenewdevice(BLEAdvertisedDevice &dev);
  static int DetectNewDevices(BLEScanResults &newDevices, BLEScanResults &oldDevices);
  static void findDeviceTask(void *pvParameters);
};
#endif
