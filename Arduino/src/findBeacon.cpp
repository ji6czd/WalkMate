#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Arduino.h"
#include <M5Stack.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <vector>
#include "findBeacon.hpp"
#include "soundOut.hpp"
using namespace std;

const int scanTime = 2; //In seconds

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
	void onResult(BLEAdvertisedDevice advertisedDevice) {
		//Serial.printf("Advertised Device: %s \r\n", advertisedDevice.getManufacturerData().c_str());
	}
};

BLEScan* pBLEScan;

void findBeacon::begin()
{
  Serial.println("Scanning...");
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  xTaskCreatePinnedToCore(findDeviceTask, "findDeviceTask", 4096, NULL, 1, NULL,1);
}

bool findBeacon::isBeacon(string data)
{
	const string iBeaconHeader = {0x4c, 0x00, 0x02, 0x15}; // Apple iBeacon
	if (data.find(iBeaconHeader) == 0) {
		return true;
	}
	return false;
}

bool findBeacon::Announcenewdevice(BLEAdvertisedDevice &dev)
{
	string ManufactureData = dev.getManufacturerData();;
	if (isBeacon(ManufactureData)) {
		Serial.println("Beacon found.");
		sOut.beep(1500, 10);
	}
}

int findBeacon::DetectNewDevices(BLEScanResults &newDevices, BLEScanResults &oldDevices)
{
	BLEAdvertisedDevice dev;
	int found=0;
	for (int i=0; i < newDevices.getCount(); i++) {
		dev = newDevices.getDevice(i);
		// Checking oldDevices
		int j;
		for (j=0; j < oldDevices.getCount(); j++) {
			if (dev.getAddress().equals(oldDevices.getDevice(j).getAddress())) {
				break;
			}
		}
		if (j >= oldDevices.getCount()) {
				// New device found
			Announcenewdevice(dev);
			found++;
		}
	}
	return found;
}

void findBeacon::findDeviceTask(void *pvParameters) {
	static BLEScanResults existDevices;
	BLEScanResults foundDevices;
	for (;;) {
		foundDevices = pBLEScan->start(scanTime);
		//Serial.print("Devices found: ");
		//Serial.println(foundDevices.getCount());
		DetectNewDevices(foundDevices, existDevices);
		existDevices = std::move(foundDevices);
	}
}

