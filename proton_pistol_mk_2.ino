// To control the pistol tip LED
#include <FastLED.h>

// The following are used for the pistol tip LED
#define NUM_LEDS 1
#define DATA_PIN 6
CRGB leds[NUM_LEDS];
long randomColor;
long randomBrightness; 

// On to the pack LED
int pack_anode = 3;
int red = 7; //The red pin is the one next to the anode, with no other pin
int blue = 8; //The blue pin is the furthest from the anode
int pack;
bool pack_on = HIGH;
bool alt_mode;

//This is the trigger button
int trigger = 2;
bool long_press = false;
bool button_active = false;
long button_timer = 0;
long long_press_time = 200;

void setup() {
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  leds[0] = CRGB::Black;
  FastLED.show();
  //Set up trigger for input
  pinMode(trigger, INPUT);
  //Set up pack LED for output
  pinMode(pack_anode, OUTPUT);
  randomSeed(analogRead(0));
  alt_mode = digitalRead(trigger);
  if (alt_mode) {
    pack = blue;
  } else {
    pack = red;
  }
  pinMode(pack, OUTPUT);
  digitalWrite(pack, LOW);
  for (int i = 0; i < 5; i++){
    digitalWrite(pack_anode, pack_on);
    delay(250);
    pack_on = !pack_on; 
  }
}

void loop() {
  

}
