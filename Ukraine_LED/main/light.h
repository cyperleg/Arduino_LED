#include <FastLED.h>

#define LED_NUM 193  //191 + 2
#define BUILDING_INDEX 109
#define BLUE_LED_INDEX 123
#define YELLOW_LED_INDEX 122
#define PIN 14

#define SIZE_ROCKETS 8

CRGB leds[LED_NUM];

CRGB rocket_color = CRGB(204, 0, 0);
CRGB shield_color = CRGB(255, 255, 153);

DEFINE_GRADIENT_PALETTE( my_gp ) {
  0,   255,  255,  51,   
  51,    255,  153,  51,   
  102,   255,  255,  102,  
  153,   255,  128,  0,
  204,  255, 255, 153,
  255, 255, 255, 204};

CRGBPalette256 myPal = my_gp;
uint8_t led_up_brightness[LED_NUM - BUILDING_INDEX];

struct Parts
{
  uint8_t start;
  uint8_t size;
};

Parts rockets[] = {
  {8, 4},
  {15, 9},
  {28, 11},
  {48, 9},
  {60, 9},
  {73, 8},
  {84, 10},
  {102, 4}
};

Parts shield[] = {
  {1, 7},
  {12, 3},
  {24, 4},
  {39, 9},
  {57, 3},
  {69, 4},
  {81, 3},
  {94, 8},
  {106, 3}
};


void shield_proceed(int index_shield){
  if (index_shield == 0) fill_solid(leds, shield[index_shield].size, shield_color);  
  fill_solid(leds + shield[index_shield + 1].start, shield[index_shield + 1].size, shield_color);
}

int index_parts = 0;
int index_rocket = 0;

bool start_rockets(){
  if (index_parts < SIZE_ROCKETS){
    if (index_rocket < rockets[index_parts].start) index_rocket = rockets[index_parts].start;
    if (index_rocket < rockets[index_parts].start + rockets[index_parts].size){
      leds[index_rocket] = rocket_color;
      if (index_rocket - 1 >= rockets[index_parts].start) leds[index_rocket - 1] = CRGB::Black;
      index_rocket ++;
    }
    else{
      shield_proceed(index_parts);
      leds[index_rocket] = CRGB::Black;
      index_parts ++;
    }
    FastLED.show();
    return false;
  }
  else return true;
}

void init_led_up_brightness(){
  for(int i = 0; i < LED_NUM - BUILDING_INDEX; i ++) led_up_brightness[i] = 128;
}

CHSV temp_color_up;
int temp_up_light_delta;

void building_up(){
  for (int i = BUILDING_INDEX; i < LED_NUM; i ++){
    temp_color_up = rgb2hsv_approximate(ColorFromPalette(myPal, random(255)));
    temp_up_light_delta = random(7);
    if(random(1)){
      if (led_up_brightness[i - BUILDING_INDEX] + temp_up_light_delta > 255) led_up_brightness[i - BUILDING_INDEX] = 255;
      else led_up_brightness[i - BUILDING_INDEX] += temp_up_light_delta;
    }
    else{
      if (led_up_brightness[i - BUILDING_INDEX] - temp_up_light_delta < 0) led_up_brightness[i - BUILDING_INDEX] = 0;
      else led_up_brightness[i - BUILDING_INDEX] -= temp_up_light_delta;
    }
    temp_color_up.v = led_up_brightness[i - BUILDING_INDEX];
    leds[i] = temp_color_up;
  }
  leds[BLUE_LED_INDEX] = CRGB::Blue;
  leds[YELLOW_LED_INDEX] = CRGB::Yellow;
  FastLED.show();
}

void configure(){
  FastLED.addLeds<WS2812B, PIN>(leds, LED_NUM);
  init_led_up_brightness();
  FastLED.clear();
}

void test(){
  fill_solid(leds, LED_NUM, CRGB::Blue);
  FastLED.show();
}