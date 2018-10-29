#ifndef _FIND_BEACON_HPP_
#define _FIND_BEACON_HPP_

class findBeacon {
public:
  boid begin();
private:
  bool isBeacon(string data);
  bool Announcenewdevice(BLEAdvertisedDevice &dev);
  int DetectNewDevices(BLEScanResults &newDevices, BLEScanResults &oldDevices);
  void findDeviceTask(void *pvParameters);
};
