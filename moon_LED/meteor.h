#define DEBUG_METEOR

#ifdef DEBUG_METEOR
#define DEBUG_PRINT(x) \
  ; \
  Serial.print(x);
#define DEBUG_PRINTLN(x) \
  ; \
  Serial.println(x);
#else
#define DEBUG_PRINT(x) ;
#define DEBUG_PRINTLN(x) ;
#endif

#include <stdint.h>
#include <FastLED.h>

#define METEOR_PIN 14
#define R 249
#define B 249
#define G 19
#define LED_NUM 75
#define UPDATE_DELAY 45
#define LED_NUM_METEOR 60
#define LED_NUM_TRACES 15
#define LED_NUM_METEOR_TAIL 7
#define LED_NUM_METEOR_HEAD_SIDE 1  // 1 is minimum size
#define METEOR_TIME 50000
#define EXPLOSION_TIME 10000
#define MAX_BRIGHTNESS 100
#define FADE_SPEED 1  // 1 is minimum

CRGB meteor_leds[LED_NUM];

int8_t meteor_LED_brightness[LED_NUM];
byte traces_LED_brightness[LED_NUM_TRACES];
int currentLED;
byte brightnessR = map(R, 0, 255, 0, MAX_BRIGHTNESS);
byte brightnessG = map(G, 0, 255, 0, MAX_BRIGHTNESS);
byte brightnessB = map(B, 0, 255, 0, MAX_BRIGHTNESS);
bool flag_stage_2 = false;
bool flag_stage_3 = false;

void set_LED_brightness(int i, uint8_t brightness) {
  uint8_t r, g, b;
  r = map(brightness, 0, 255, 0, brightnessR);
  g = map(brightness, 0, 255, 0, brightnessG);
  b = map(brightness, 0, 255, 0, brightnessB);
  meteor_leds[i] = CRGB(r, g, b);
}

void reset_brightness() {  // use when mode changed
  for (int i = 0; i < LED_NUM; i++) {
    meteor_LED_brightness[i] = 0;  //255 stands for no brightness
  }
}


void close_meteor() {
  FastLED.clear();
  FastLED.show();
  reset_brightness();
  flag_stage_2 = false;
  flag_stage_3 = false;
}

void configure_meteor() {
  FastLED.addLeds<WS2812B, METEOR_PIN>(meteor_leds, LED_NUM);
  FastLED.clear();
  for (int i = 0; i < LED_NUM; i++) {
    meteor_LED_brightness[i] = 0;
    meteor_leds[i] = CRGB(R, G, B);
    set_LED_brightness(i, 0);  // set brightness to 0
  }
  for (int i = 0; i < LED_NUM_TRACES; i++) {
    traces_LED_brightness[i] = 255;
  }
}

void meteor_head(unsigned long tCurrent) {
  currentLED = map(tCurrent, 0, METEOR_TIME, 0, LED_NUM_METEOR);
  if (meteor_LED_brightness[currentLED] != MAX_BRIGHTNESS) {
    meteor_LED_brightness[currentLED] = MAX_BRIGHTNESS;
    for (int i = 1; i <= LED_NUM_METEOR_HEAD_SIDE; i++) {
      if (currentLED + i < LED_NUM_METEOR) meteor_LED_brightness[currentLED + i] = int(
                                             (MAX_BRIGHTNESS / (LED_NUM_METEOR_HEAD_SIDE + 1)) * (1 + LED_NUM_METEOR_HEAD_SIDE - i));
      if (currentLED - i >= 0) meteor_LED_brightness[currentLED - i] = int(
                                 (MAX_BRIGHTNESS / (LED_NUM_METEOR_HEAD_SIDE + 1)) * (1 + LED_NUM_METEOR_HEAD_SIDE - i));
    }
  }
}

void head_smooth_move() {
  for (int i = currentLED + 1; i <= currentLED + LED_NUM_METEOR_HEAD_SIDE + 1; i++) {
    if (meteor_LED_brightness[i] < MAX_BRIGHTNESS - FADE_SPEED) {
      meteor_LED_brightness[i] += FADE_SPEED;
      if (meteor_LED_brightness[i] >= MAX_BRIGHTNESS) meteor_LED_brightness[i] = MAX_BRIGHTNESS - 1;
    }
  }
}

void head_smooth_move_alt() {
  for (int i = currentLED + 1; i <= currentLED + int(LED_NUM_METEOR_TAIL / 2) + LED_NUM_METEOR_HEAD_SIDE; i++) {
    if (i >= LED_NUM_METEOR - 1) {
      flag_stage_2 = true;
    } else {
      if (meteor_LED_brightness[i - 1] > int(MAX_BRIGHTNESS / (int(LED_NUM_METEOR_TAIL / 2) + LED_NUM_METEOR_HEAD_SIDE))) {
        if (meteor_LED_brightness[i] < MAX_BRIGHTNESS - FADE_SPEED) {
          meteor_LED_brightness[i] += FADE_SPEED;
          if (meteor_LED_brightness[i] >= MAX_BRIGHTNESS) meteor_LED_brightness[i] = MAX_BRIGHTNESS - 1;
        }
      }
    }
  }
}

void tail_fade() {
  for (int i = 0; i < currentLED - LED_NUM_METEOR_HEAD_SIDE; i++) {
    meteor_LED_brightness[i] -= FADE_SPEED;
    if (meteor_LED_brightness[i] < 0) meteor_LED_brightness[i] = 0;
  }
}

bool check_brightness() {
  for (int i = 0; i < LED_NUM_METEOR; i++) {
    if (meteor_LED_brightness[i] != 0) return false;
  }
  return true;
}

void meteor_finish(uint32_t tCurrent) {
  long long int delta_brightness = map(tCurrent, METEOR_TIME, METEOR_TIME + EXPLOSION_TIME * 0.2, 0, MAX_BRIGHTNESS);
  for (int i = 0; i < LED_NUM_METEOR; i++) {
    if (meteor_LED_brightness[i] == 0) {
      continue;
    } else {
      if (meteor_LED_brightness[i] - delta_brightness < 0) meteor_LED_brightness[i] = 0;
      else meteor_LED_brightness[i] -= delta_brightness;
    }
  }
  flag_stage_3 = check_brightness();
}

void meteor_traces(uint32_t tCurrent) {
  long long int delta_brightness = map(tCurrent, METEOR_TIME, METEOR_TIME + EXPLOSION_TIME * 0.8, 0, MAX_BRIGHTNESS);
  for (int i = LED_NUM_METEOR; i < LED_NUM_METEOR + LED_NUM_TRACES; i++) {
    if (traces_LED_brightness[i - LED_NUM_METEOR] - delta_brightness < 0) meteor_LED_brightness[i] = 0;
    else traces_LED_brightness[i - LED_NUM_METEOR] -= delta_brightness;
  }
}

bool meteor_LED_proceed(unsigned long tStart) {
  uint32_t tCurrent = millis() - tStart;
  if (tCurrent > METEOR_TIME + EXPLOSION_TIME) return true;
  else {
    if (flag_stage_3) {
      meteor_traces(tCurrent);
      for (int i = LED_NUM_METEOR; i < LED_NUM_METEOR + LED_NUM_TRACES; i++) {
        DEBUG_PRINT(String(meteor_LED_brightness[i]) + " ");
        set_LED_brightness(i, meteor_LED_brightness[i]);
      }
      DEBUG_PRINTLN();
    } else {
      if (flag_stage_2) {
        meteor_finish(tCurrent);
      } else {
        meteor_head(tCurrent);
        //head_smooth_move();
        head_smooth_move_alt();
        tail_fade();
      }


      for (int i = 0; i < LED_NUM_METEOR; i++) {
        DEBUG_PRINT(String(meteor_LED_brightness[i]) + " ");
        set_LED_brightness(i, meteor_LED_brightness[i]);
      }
      DEBUG_PRINTLN();
    }

    FastLED.show();
    delay(UPDATE_DELAY);
    return false;
  }
}
