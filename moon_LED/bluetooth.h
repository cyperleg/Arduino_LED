#include <BTAddress.h>
#include <BTAdvertisedDevice.h>
#include <BTScan.h>
#include <BluetoothSerial.h>

BluetoothSerial ESP_BT;
uint8_t incoming;


void configure_BT() {
  ESP_BT.begin("Moon Exhibition");
}


int8_t mode_BT() {
  int a = ESP_BT.read();
  //Serial.println(a);
  if (a >= 48) return a - 48;
  return -1;
}

bool check_BT() {
  return ESP_BT.available();
}


void check_mode() {
  // Serial BT handling/ changing global variable MODE
}