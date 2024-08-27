#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

class ServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    Serial.println("Device connected");
  }

  void onDisconnect(BLEServer *pServer)
  {
    Serial.println("Device disconnected");
  }
};

class CharacteristicCallbacks : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *pCharacteristic)
  {
    String value = pCharacteristic->getValue();

    if (value.length() > 0)
    {
      Serial.println("Received value: ");
      for (int i = 0; i < value.length(); i++)
      {
        Serial.print(value[i]);
      }

      Serial.println();
    }
  }
};

void setup()
{
  Serial.begin(115200);

  // Create the BLE Device
  BLEDevice::init("Scoobies Penguin");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE |
          BLECharacteristic::PROPERTY_NOTIFY |
          BLECharacteristic::PROPERTY_INDICATE);

  pCharacteristic->setCallbacks(new CharacteristicCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  Serial.println("BLE device ready");
}

void loop()
{
}
