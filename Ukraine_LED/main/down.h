#include <FastLED.h>

#define DOWN_LED_NUM 45  //43 + 2
#define DOWN_PIN 12

CRGB down_leds[DOWN_LED_NUM];

DEFINE_GRADIENT_PALETTE( my_down_gp ) {
  0,   255,  255,  51,   
  51,    255,  153,  51,   
  102,   255,  255,  102,  
  153,   255,  128,  0,
  204,  255, 255, 153,
  255, 255, 255, 204};

CRGBPalette256 my_down_Pal = my_down_gp;
uint8_t led_down_brightness[LED_NUM - BUILDING_INDEX];

void init_led_down_brightness(){
  for(int i = 0; i < LED_NUM - BUILDING_INDEX; i ++) led_down_brightness[i] = 128;
}

CHSV temp_color_down;
int temp_down_light_delta;

void building_down(){
  for (int i = 0; i < DOWN_LED_NUM; i ++){
    temp_color_down = rgb2hsv_approximate(ColorFromPalette(myPal, random(255)));
    temp_down_light_delta = random(7);
    if(random(1)){
      if (led_down_brightness[i] + temp_down_light_delta > 255) led_down_brightness[i] = 255;
      else led_down_brightness[i] += temp_down_light_delta;
    }
    else{
      if (led_down_brightness[i] - temp_down_light_delta < 0) led_down_brightness[i] = 0;
      else led_down_brightness[i] -= temp_down_light_delta;
    }
    temp_color_down.v = led_down_brightness[i];
    leds[i] = temp_color_down;
  }
  for(int i = 66; i <= 69; i ++) leds[i] = CRGB(255, 255, 153);
  FastLED.show();
}


void down_configure(){
  FastLED.addLeds<WS2812B, DOWN_PIN>(down_leds, DOWN_LED_NUM);
  init_led_down_brightness();
  FastLED.clear();
}

void down_test(){
  fill_solid(down_leds, DOWN_LED_NUM, CRGB::Blue);
  FastLED.show();
}