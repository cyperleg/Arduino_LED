#include <FastLED.h>

#define SPARK_PIN 26
#define LED_COUNT 72
#define LED_SMALL_CLUSTERS 6
#define COUNT_SMALL_CLUSTERS 7
#define LED_MED_CLUSTERS 10
#define LED_BIG_CLUSTERS 20
#define SPARK_TIME 120000
#define MAX_BRIGHTNESS 150
#define DELAY 45
#define PULSES_MAX 70
#define PULSES_MIN 40
#define R 255
#define B 255
#define G 255

CRGB spark_leds[LED_COUNT];

byte spark_brightness[LED_COUNT];

byte sparkbrightnessR = map(R, 0, 255, 0, MAX_BRIGHTNESS);
byte sparkbrightnessG = map(G, 0, 255, 0, MAX_BRIGHTNESS);
byte sparkbrightnessB = map(B, 0, 255, 0, MAX_BRIGHTNESS);

void CUM_spark(int i, uint8_t brightness) {
  uint8_t r, g, b;
  r = map(brightness, 0, 255, 0, sparkbrightnessR);
  g = map(brightness, 0, 255, 0, sparkbrightnessB);
  b = map(brightness, 0, 255, 0, sparkbrightnessG);
  spark_leds[i] = CRGB(r, g, b);
}

void close_spark() {
  FastLED.clear();
  FastLED.show();
  for (int i = 0; i < LED_COUNT; i++) {
    spark_brightness[i] = 0;
  }
}

void configure_spark() {
  FastLED.addLeds<WS2812B, SPARK_PIN>(spark_leds, LED_COUNT);
  FastLED.clear();
  for (int i = 0; i < LED_COUNT; i++) {
    spark_brightness[i] = 0;
  }
}

bool spark(uint32_t time_start) {
  uint32_t time_current = millis() - time_start;

  if (time_current > SPARK_TIME) return true;
  else {
    for (int i = 0; i < LED_COUNT; i++) {
      Serial.print(String(spark_brightness[i]) + " ");
      bool rnd = random(0, 2);
      byte a = random(2, 20);
      Serial.println(String(rnd));
      if (rnd) {
        if (spark_brightness[i] + a > 255) spark_brightness[i] = 255;
        else spark_brightness[i] = spark_brightness[i] + a;
      } else {
        if (spark_brightness[i] - a < 0) spark_brightness[i] = 0;
        else spark_brightness[i] = spark_brightness[i] - a;
      }
      CUM_spark(i, spark_brightness[i]);
    }
    Serial.println();
    FastLED.show();
    delay(DELAY);
    return false;
  }
}
