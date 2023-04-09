#include <Arduino.h>
#include <FastLED.h>
#include <TFT_eSPI.h>

#define NUM_LEDS 30
#define DATA_PIN 12
#define BUTTON_LEFT 35
#define BUTTON_RIGHT 0

CRGB leds[NUM_LEDS];

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite spr = TFT_eSprite(&tft);

void setup()
{
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);

  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_DARKCYAN);
  tft.setTextColor(TFT_WHITE, TFT_DARKCYAN);
  tft.setTextSize(1);
  tft.setTextFont(4);
  tft.setCursor(0, 0);
  tft.println("Loaded!");
  tft.println(__TIME__);

  spr.setTextFont(7);  
  spr.createSprite(240, 135);
  spr.setTextColor(TFT_WHITE, TFT_DARKCYAN);

  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(50);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 128);
  FastLED.clear();
  FastLED.show();
}

int ledCount = 0;
int lastBtnRightState = HIGH;
int lastBtnLeftState = HIGH;

void loop()
{
  spr.fillRect(0, 0, 240, 135, TFT_DARKCYAN);
  spr.setTextSize(2);
  spr.setCursor(10, 10);
  spr.print(ledCount);

  spr.setTextSize(.5);
  spr.setCursor(160, 85);
  spr.print(NUM_LEDS);

  spr.pushSprite(0, 0);  

  int btnLeftState = digitalRead(BUTTON_LEFT);
  int btnRightState = digitalRead(BUTTON_RIGHT);

  if (btnLeftState != lastBtnLeftState)
  {
    lastBtnLeftState = btnLeftState;
    if (btnLeftState == LOW)
    {
      ledCount--;
      if (ledCount < 0)
        ledCount = NUM_LEDS;
    }
  }

  if (btnRightState != lastBtnRightState)
  {
    lastBtnRightState = btnRightState;
    if (btnRightState == LOW)
    {
      ledCount++;
      if (ledCount > NUM_LEDS)
        ledCount = 0;
    }
  }

  EVERY_N_MILLISECONDS(100)
  {
    FastLED.clear();
    if (ledCount > 0)
      fill_rainbow(leds, ledCount, 0, 255 / NUM_LEDS);
    FastLED.show();
  }
}