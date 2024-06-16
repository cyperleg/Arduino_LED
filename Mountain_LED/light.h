#include <FastLED.h>

#define LED_NUM 175
#define PIN 12
#define STRIP_LED 29

//Define palette
DEFINE_GRADIENT_PALETTE( template_gp ) {
  0,   19,  107,  127,   //black
  80,    121,  176,  187,   //red
  160,   179,  240,  241,   //bright yellow
  255,   249,  214,  214 }; //full white

DEFINE_GRADIENT_PALETTE( polar_gp ) {
  0,   102,  255,  178,   //black
  80,    0,  204,  102,   //red
  160,   255,  0,  127,   //bright yellow
  255,   255,  255,  255 }; //full white

CRGBPalette256 myPal = polar_gp;
uint8_t color_index[STRIP_LED];
//---------------------------------------


//Creating stylelist
struct Part
{
  uint8_t size;
  CRGB color;
};

Part parts[] = {
  {3, CRGB::Maroon},
  {8, CRGB::Teal},
  {14, CRGB::LightYellow},
  {4, CRGB::Maroon},
  {3, CRGB::ForestGreen},
  {13, CRGB::Orange},
  {9, CRGB::Teal},
  {2, CRGB::Maroon},
  {7, CRGB::Teal}, 
  {6, CRGB::ForestGreen},
  {4, CRGB::Yellow},
  {4, CRGB::Orange},
  {2, CRGB::Teal},
  {2, CRGB::Teal},
  {3, CRGB::RosyBrown},
  {5, CRGB::RosyBrown},
  {3, CRGB::RosyBrown},
  {5, CRGB::Maroon},
  {5, CRGB::LightYellow},
  {2, CRGB::LightYellow},
  {3, CRGB::LightYellow},
  {5, CRGB::Maroon},
  {5, CRGB::Brown},
  {5, CRGB::Red},
  {5, CRGB::Brown},
  {6, CRGB::Teal},
  {4, CRGB::LightYellow},
  {2, CRGB::Red},
  {3, CRGB::Red},
  {3, CRGB::Maroon}
};

int size_parts = sizeof(parts) / sizeof(Part);
//---------------------------------------


//LED array
CRGB leds[LED_NUM];
//---------------------------------------


//Servise var
//bool mountain_flag = false;
//bool art_stop_flag = false;
//---------------------------------------


// Color pallete functions
void set_default_color_index() {
  for(int k = 0; k < STRIP_LED; k ++) color_index[k] = k;
}

void color_index_increment() {
  for(int k = 0; k < STRIP_LED; k ++) {
      if (++color_index[k] >= STRIP_LED) color_index[k] = 0;
  }
}
//---------------------------------------


//Color struct function
int i = STRIP_LED;
int temp = i;

void set_color(uint8_t size, CRGB color) {
  for(i; i < temp + size; i ++) leds[i] = color;
  temp = i;
}
//---------------------------------------


//Usage main functions
void start_polar() {
  for (int j = 0; j < STRIP_LED; j++) leds[j] = ColorFromPalette(myPal, map(color_index[j], 0, STRIP_LED, 0, 255));  
  color_index_increment();
  FastLED.show();
}

void art_stop() {
  for(int j = 0; j < LED_NUM; j ++) leds[j] = CRGB::Black;
  FastLED.show();
}

void stop_polar(){
  for(int j = 0; j < STRIP_LED; j ++) leds[j] = CRGB::Black;
  FastLED.show();
}

void start_mountain() {
  i = STRIP_LED;
  temp = i;
  for(int j = 0; j < size_parts; j ++) set_color(parts[j].size, parts[j].color);
  FastLED.show();
}

void configure() {
  FastLED.addLeds<WS2812B, PIN>(leds, LED_NUM);
  FastLED.clear();
  start_mountain();
  set_default_color_index();
}
//---------------------------------------


//Test function
void fill_red() {
  for(int j = 0; j < LED_NUM; j ++) leds[j] = CRGB::Red;
  FastLED.show();
}
//---------------------------------------
