#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "light.h"
#include "down.h"

//Global mode var
uint8_t mode = 2;
//---------------------------------------


//BLE var
#define SERVICE_UUID              "9a8ca9ef-e43f-4157-9fee-c37a3d7dc12d" // ID сервиса
#define ELEM_UUID              "cc46b944-003e-42b6-b836-c4246b8f19a0" // ID характеристики

#define DEVINFO_UUID              (uint16_t)0x180a
#define DEVINFO_MANUFACTURER_UUID (uint16_t)0x2a29
#define DEVINFO_NAME_UUID         (uint16_t)0x2a24
#define DEVINFO_SERIAL_UUID       (uint16_t)0x2a25

#define DEVICE_MANUFACTURER "ESP32" // Имя "Производителя"
#define DEVICE_NAME         "Mountain LED" // Имя устройства
//---------------------------------------


//BLE class
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      // Обработка подключения телефона к устройству
      Serial.println("Connected");
    };

    void onDisconnect(BLEServer* pServer) {
      // Обработка отключения
      Serial.println("Disconnected");
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      if (value.length() > 0) {
        // Проверка, что входные данные представляют собой числовое значение
        for (int i = 0; i < value.length(); i++) {
          if (!isdigit(value[i])) {
            Serial.println("Invalid data format. Only numerical values are allowed.");
            return;
          }
        }

        // Преобразование строки в числовое значение
        int numericValue = atoi(value.c_str());
        if (numericValue >= 0 && numericValue <= 2) mode = numericValue;
        else mode = 2;
        pCharacteristic->setValue("Mode: " + char(mode));
        
        Serial.println("Mode: " + String(mode));
      }
    }
};
//---------------------------------------


//Main programm
void setup() {
  //Strip Setup
  configure();
  down_configure();
  //---------------------------------------
  //Serial output
  Serial.begin(115200);
  //---------------------------------------


  //BLE setup
  Serial.println("Starting BLE work!");

  BLEDevice::init(DEVICE_NAME); // Инициализация девайса
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         ELEM_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  pCharacteristic->setCallbacks(new MyCallbacks());
  pCharacteristic->setValue("Mode: " + char(mode));
  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  Serial.println("Characteristic defined! Now you can read it in your phone!");
  //---------------------------------------
}

void loop() {
  //Serial.println("Main loop");
  //delay(100);
  
  switch(mode) {
    case 0:
      Serial.println("Case 0 ");
      start_rockets();
      break;
    case 1:
      Serial.println("Case 1 ");
      building_up();
      building_down();
      break;
  }
  
  //fill_red();
  delay(1000 / 60);
}
//---------------------------------------


