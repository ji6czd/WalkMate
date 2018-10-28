#include <M5Stack.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <vector>
#include "beaconFound.h"

using namespace std;

int scanTime = 3; //In seconds

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
	void onResult(BLEAdvertisedDevice advertisedDevice) {
		//Serial.printf("Advertised Device: %s \r\n", advertisedDevice.getManufacturerData().c_str());
	}
};

BLEScan* pBLEScan;

void setup()
{
	M5.begin();
	M5.Speaker.begin();
  Serial.println("Scanning...");
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
	xTaskCreatePinnedToCore(findDeviceTask, "findDeviceTask", 8192, NULL, 1, NULL,1);
}

bool isBeacon(string data) {
  const string beaconHeader = {0x4c, 0x00, 0x02, 0x15};
	if (data.find(beaconHeader) == 0) {
		return true;
	}
	return false;
}

bool Announcenewdevice(BLEAdvertisedDevice &dev)
{
	string ManufactureData = dev.getManufacturerData();;
	if (isBeacon(ManufactureData)) {
		M5.Speaker.playMusic(beaconFound_raw, beaconFound_raw_len, 8000);
		Serial.println("Beacon found.");
	}
}

int DetectNewDevices(BLEScanResults &newDevices, BLEScanResults &oldDevices)
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

void findDeviceTask(void *pvParameters) {
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

void loop() {
	M5.update();
}
