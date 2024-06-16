#include <FastLED.h>

#define MOON_PIN 27
#define R 255
#define B 255
#define G 255
   //158
#define MOON_TIME 600000
#define FADE_COUNT 4
#define MAX_BRIGHTNESS 150
#define DELAY 45

CRGB moon_leds[LED_NUM_MOON];
uint8_t moon_leds_brightness[LED_NUM_MOON];
bool flag = true;

byte moonbrightnessR = map(R, 0, 255, 0, MAX_BRIGHTNESS);
byte moonbrightnessG = map(G, 0, 255, 0, MAX_BRIGHTNESS);
byte moonbrightnessB = map(B, 0, 255, 0, MAX_BRIGHTNESS);

void close_moon() {
  FastLED.clear();
  FastLED.show();
  for (int i = 0; i < LED_NUM_MOON; i++) {
    moon_leds_brightness[i] = 0;
  }
}

void CUM(int i, uint8_t brightness) {
  uint8_t r, g, b;
  r = map(brightness, 0, 255, 0, moonbrightnessR);
  g = map(brightness, 0, 255, 0, moonbrightnessB);
  b = map(brightness, 0, 255, 0, moonbrightnessG);
  moon_leds[i] = CRGB(r, g, b);
}

void configure_moon() {
  FastLED.addLeds<WS2812B, MOON_PIN>(moon_leds, LED_NUM_MOON);
  FastLED.clear();
  for (int i = 0; i < LED_NUM_MOON; i++) {
    moon_leds_brightness[i] = 0;
    moon_leds[i] = CRGB(R, G, B);
    CUM(i, 0);
  }
}

bool moon(uint32_t time_start) {
  uint32_t time_current = millis() - time_start;

  if (time_current >= MOON_TIME) return true;
  else {

    // define max led moon + fade
    uint16_t max_led = (LED_NUM_MOON / 2) + FADE_COUNT;
    // define current led that depends on time
    uint16_t currentLED = map(time_current, 0, MOON_TIME / 2, 0, max_led);
    // define delta brightness
    // для одного светодиода яркость остается неизменной, то есть он должен перейти с 0 до максимальной яркости
    // но время разниться поскольку длина хвоста меняется и хвост тоже должен продолжать светится то есть за один проход яркость меняется от 0 до макс / длину хвоста
    uint8_t delta_brightness = map(time_current / (max_led), 0, MOON_TIME / (max_led), 0, MAX_BRIGHTNESS / FADE_COUNT);

    if (moon_leds_brightness[LED_NUM_MOON / 2] == 255) flag = false;
    if (moon_leds_brightness[LED_NUM_MOON / 2] == 0) flag = true;

    // block from 0 to MAX_BRIGHTNESS for top half of circle
    if (flag) {
      for (int i = currentLED; i >= 0 && i >= currentLED - FADE_COUNT && i < LED_NUM_MOON / 2; i--) {
        Serial.print(String(i) + "-" + String(moon_leds_brightness[i]) + " ");
        if (moon_leds_brightness[i] + delta_brightness >= 255) moon_leds_brightness[i] = 255;
        else moon_leds_brightness[i] += delta_brightness;
        CUM(i, moon_leds_brightness[i]);
        if (moon_leds_brightness[LED_NUM_MOON - i] + delta_brightness >= 255) moon_leds_brightness[LED_NUM_MOON - i] = 255;
        else moon_leds_brightness[LED_NUM_MOON - i] += delta_brightness;
        CUM(LED_NUM_MOON - i, moon_leds_brightness[LED_NUM_MOON - i]);
      }
    } else {
      for (int i = currentLED; i >= 0 && i >= currentLED - FADE_COUNT && i < LED_NUM_MOON / 2; i--) {
        //Serial.print(String(moon_leds_brightness[i]) + " ");
        if (moon_leds_brightness[i] - delta_brightness <= 0) moon_leds_brightness[i] = 0;
        else moon_leds_brightness[i] -= delta_brightness;
        CUM(i, moon_leds_brightness[i]);
        if (moon_leds_brightness[LED_NUM_MOON - i] - delta_brightness <= 255) moon_leds_brightness[LED_NUM_MOON - i] = 0;
        else moon_leds_brightness[LED_NUM_MOON - i] -= delta_brightness;
        CUM(LED_NUM_MOON - i, moon_leds_brightness[LED_NUM_MOON - i]);
      }
    }

    Serial.println();
    FastLED.show();
    delay(DELAY);
    return false;
  }
}