#include "meteor.h"
#include "bluetooth.h"
#include "moon.h"
#include "sparks.h"

#define MAX_MODE 3

unsigned long MODE_START = millis();
uint8_t MODE = 1;


void setup() {
  Serial.begin(115200);
  configure_BT();
  configure_meteor();
  configure_moon();
  configure_spark();
}

void next_mode() {
  if (MODE == MAX_MODE) MODE = 1;
  else MODE++;
  close_all();
  MODE_START = millis();
}

void close_all() {
  close_meteor();
  close_moon();
  close_spark();
}

void mode_handler() {
  // TODO ONLY if MODE was changed do:

  // handle modes
  switch (MODE) {
    case 0:
      close_all();
      break;
    case 1:
      if (meteor_LED_proceed(MODE_START)) next_mode();
      break;
    case 2:
      if (moon(MODE_START)) next_mode();
      break;
    case 3:
      if (spark(MODE_START)) next_mode();
  }
}

void loop() {
  if (!check_BT()) {
    check_mode();
  } else {
    int8_t check = mode_BT();
    if (check != -1) MODE = check;
    close_all();
    MODE_START = millis();
    Serial.println(MODE);
  }
  mode_handler();
}