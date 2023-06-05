#include <OneButton.h>
#include <FastLED.h>
#define TIP_PIN 5
//#define BUTTON_IN 7
CRGB tip[1];
//int TIP_PIN = 5;
int A_SIDE = 0;
int B_SIDE = 1;
int R_LED = 2;
int G_LED = 3;
int B_LED = 4;
int PACK_LED = 6;
int BUTTON_IN = 7;
int mode = 0;
int packLights[5] = {R_LED, G_LED, B_LED, R_LED, G_LED};
int sounds[5] = {8,9,10,11,12};
OneButton button;
int heatStatus = 255;  // Indicates brightness of PACK_LED. Comes back gradually on its own, or can be refreshed to full by venting
int streamHeat[4] = {15, 2, 20, 18};
int blastHeat[4] = {90, 1, 55, 35};
int regainHeat[4] = {5, 30, 10, 10};
int blastDelays[4] = {3000, 1000, 1500, 1500};
CRGB mainColors[4] = {CRGB::Chartreuse, CRGB::LimeGreen, CRGB::Blue, CRGB::Gold};
CRGB contrastColors[4] = {CRGB::DarkViolet, CRGB::SeaGreen, CRGB::Navy, CRGB::Yellow};
int previousBrightness = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(A_SIDE, INPUT);
  pinMode(B_SIDE, INPUT);
  pinMode(BUTTON_IN, INPUT);
  pinMode(R_LED, OUTPUT);
  digitalWrite(R_LED, HIGH);
  pinMode(G_LED, OUTPUT);
  digitalWrite(G_LED, HIGH);
  pinMode(B_LED, OUTPUT);
  digitalWrite(B_LED, HIGH);
  pinMode(PACK_LED, OUTPUT);
  analogWrite(PACK_LED, 255);
  for (int i=0; i < 5; i++){
    pinMode(sounds[i], OUTPUT);
    digitalWrite(sounds[i], HIGH);
  }
  if (digitalRead(A_SIDE)){
    if (digitalRead(BUTTON_IN)){
      mode = 1;
    } else{
      mode = 0;
    }
  } else if (digitalRead(B_SIDE)){
    if (digitalRead(BUTTON_IN)){
      mode = 3;
    } else {
      mode = 2;
    }
  }
  int state = LOW;
  for (int i=0; i<5; i++){
    digitalWrite(packLights[mode], state);
    if (mode == 3){
      digitalWrite(packLights[4], state);
    }
    state = !state;
    delay(250);
  }
  FastLED.addLeds<WS2812, TIP_PIN, GRB>(tip, 1);
  leds[0] = CRGB::Black;
  FastLED.show();
  button = OneButton(BUTTON_IN, false, false);
  button.attachClick(shortFire);
  button.attachDoubleClick(doVent);
  button.attachLongPressStart(startStream);
  button.attachDuringLongPress(stream);
  button.attachLongPressStop(endStream);
}

static void shortFire(){
    digitalWrite(sounds[mode], LOW);
    delay(126);
    digitalWrite(sounds[mode], HIGH);
    tip[0] = mainColors[mode];
    tip[0].maximizeBrightness();
    FastLED.show();
    delay(blastDelays[mode]);
    tip[0] = CRGB::Black;
    FastLED.show();
}

static void doVent(){
  if (mode != 1){  // mode 1 is slime mode, which doesn't require a venting. venting will do nothing, in this case as it regains it fast enough
    digitalWrite(sounds[4], LOW);
    delay(126);
    digitalWrite(sounds[4], HIGH);
    analogWrite(PACK_LED, 255);
    delay(5000);
  }
}

static void setTip(){
  long contrast = random(200);
  long brightness = random(128,256);
  if (contrast % 5 == 0) {
    tip[0] = contrastColors[mode];
  } else {
    tip[0] = mainColors[mode];
  }
  //tip[0].fadeLightBy(uint8_t fadefactor)
  tip[0].maximizeBrightness();
  tip[0].fadeLightBy(256-brightness);
  FastLED.show();
}

static void startStream(){
  digitalWrite(sounds[mode], LOW);
  heatStatus -= streamHeat[mode];
  analogWrite(PACK_LED, heatStatus);
  setTip();
}

static void stream(){
  heatStatus -= streamHeat[mode];
  if (heatStatus < 0){
    analogWrite(PACK_LED, 0);
    tip[0] = CRGB::Black;
    FastLED.show();
    digitalWrite(sounds[mode], HIGH);
    doVent();
  } else {
    analogWrite(PACK_LED, heatStatus);
    setTip();
  }
}

static void endStream(){
  heatStatus -= streamHeat[mode];
  analogWrite(PACK_LED, heatStatus);
  digitalWrite(sounds[mode], HIGH);
  tip[0] = CRGB::Black;
  FastLED.show();
}

void loop() {
  // put your main code here, to run repeatedly:
  button.tick();
  heatStatus += regainHeat[mode];
  analogWrite(PACK_LED, heatStatus);
}
