#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <JC_Button.h>

#define NUM_LEDS 60

#define LED_PIN 0
#define BUTTON 2
#define ROT_A 3
#define ROT_B 4

#define INCREMENT 10;

Adafruit_NeoPixel leds = Adafruit_NeoPixel(
  NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

bool enabled = false;

int16_t red = 50;
int16_t green = 50;
int16_t blue = 50;

bool wasLowLast = false;
bool encoderInput = false;
bool cw = false;

Button button = Button(BUTTON);

void setup() {
  GIMSK = 0b00100000;
  PCMSK = 0b00001000;

  pinMode(ROT_A, INPUT_PULLUP);
  pinMode(ROT_B, INPUT_PULLUP);

  leds.begin();
  button.begin();

  delay(500);

  for(int i=0; i < NUM_LEDS; i++){
    leds.setPixelColor(i, leds.Color(0, 0, 0));
  }
  leds.show();
  sei();

  delay(500);
}

void loop() {
  button.read();

  if (enabled) {
    for(int i=0; i < NUM_LEDS; i++){
      leds.setPixelColor(i, leds.Color(red,green,blue));
    }
    leds.show();
  }
  if(button.wasReleased()) {
    enabled = !enabled;

    if(!enabled) {
      for(int i=0; i < NUM_LEDS; i++){
        leds.setPixelColor(i, leds.Color(0, 0, 0));
      }
      leds.show();
    }
  }
  if(encoderInput) {
    encoderInput = false;

    if(cw) {
      red += INCREMENT;
      if(red >= 255) {
        red = 255;
      }
      green += INCREMENT;
      if(green >= 255) {
        green = 255;
      }
      blue += INCREMENT;
      if(blue >= 255) {
        blue = 255;
      }
    } else {
      red -= INCREMENT;
      if(red < 0) {
        red = 0;
      }
      green -= INCREMENT;
      if(green < 0) {
        green = 0;
      }
      blue -= INCREMENT;
      if(blue < 0) {
        blue = 0;
      }
    }
  }
}

ISR(PCINT0_vect)
{
  if(!(PINB & 0b00001000)) {
    wasLowLast = true;
  }

  if(wasLowLast) {
    encoderInput = true;
    wasLowLast = false;
    if(PINB & 0b00010000) {
      cw = true;
    } else {
      cw = false;
    }
  }
}
