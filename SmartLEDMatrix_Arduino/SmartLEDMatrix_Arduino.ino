#include <Adafruit_NeoPixel.h>
#include <microDS3231.h>
#include "Symbols/Symbols.h"

#define LED_BRIGHTNESS 50
#define LED_PIN 8
#define LED_WIDTH 32
#define LED_HEIGHT 8
#define LED_COUNT (LED_WIDTH * LED_HEIGHT)
#define SPC 1
#define LENSHF 0
#define W 5
#define H 7

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
MicroDS3231 rtc;

void setup()
{
  Serial.begin(9600);

  strip.begin();
  strip.show();
  strip.setBrightness(LED_BRIGHTNESS);

  clearAllPixels();
}

String prev = "";
uint32_t col_T = strip.Color(75, 0, 255);
uint32_t col_else = strip.Color(0, 0, 0);
int count = 0;
double shft = 0;
int length_date = W*5 + 4 * SPC;
int shift_prev = 0;

void loop()
{
  for (int i = 0; i < LED_WIDTH; i++)
    light(i, 7, strip.gamma32(strip.ColorHSV(count + (256 * (i + (LED_WIDTH - 1))) % (5 * 65536))));
  drawTime(col_T, (int)shft / 10);
  delay(10);
  shft += LENSHF;
  if (shft >= 320)
    shft = 0;
  count += 256;
  if (count >= 5 * 65536)
    count = 0;
}

void drawString(String s, uint32_t color)
{
  if (s.length() <= 5)
  {
    int x = (LED_WIDTH - (s.length() * W + s.length() - 1)) / 2;
    for (int i = 0; i < s.length(); i++)
    {
      drawSymbol(s[i] - ' ', x, color, 0);
      x += W + SPC;
    }
  }
  strip.show();
}

// time visualizer
void drawTime(uint32_t col, int shift)
{
  String t = rtc.getTimeString().substring(0, 5);
  if (shift != shift_prev)
  {
    clear(0, 0, LED_WIDTH, LED_HEIGHT - 1);
    shift_prev = shift;
    int lastX = (LED_WIDTH - length_date) / 2;
    for (int i = 0; i < 5; i++)
    {
      drawSymbol(t[i] - ' ', lastX - shift, col, 0);
      lastX += W + SPC;
    }
  }
  else if (t != prev)
  {
    int lastX = (LED_WIDTH - length_date) / 2;
    for (int i = 0; i < 5; i++)
    {
      if (t[i] != prev[i])
        drawSymbol(t[i] - ' ', lastX - shift, col, 0);
      lastX += W + SPC;
    }
    prev = t;
  }
  strip.show();
}

int cycleX(int i)
{
  return (i < 0 ? LED_WIDTH - (-i % LED_WIDTH) : i % LED_WIDTH);
}

int cycleY(int j)
{
  return (j < 0 ? LED_HEIGHT - (-j % LED_HEIGHT) : j % (LED_HEIGHT - 1));
}

// set i, j pixel to color
void light(int i, int j, uint32_t color)
{
  strip.setPixelColor(LED_HEIGHT * i + (i % 2 ? (LED_HEIGHT - 1) - j : j), color);
}

// set a rectangle of pixels black
void clear(int x, int y, int w, int h)
{
  for (int i = x; i < x + w; i++)
  {
    for (int j = y; j < y + h; j++)
    {
      light(i, j, strip.Color(0, 0, 0));
    }
  }
  // strip.show();
}

// set all pixels to black
void clearAllPixels()
{
  for (int i = 0; i < strip.numPixels(); i++)
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  strip.show();
}

// *------>
// |     x
// | y
// V
// draw one symbol with code n at start with color
void drawSymbol(int n, int start, uint32_t color, int wait)
{
  for (int i = 0; i < W; i++)
  {
    for (int j = 0; j < H; j++)
    {
      if (pgm_read_byte(&Symbols[n][j][i]))
      {
        light(cycleX(i + start), cycleY(j), color);
        delay(wait);
      }
      else{
        light(cycleX(i + start), cycleY(j), strip.Color(0,0,0));
      }
    }
  }
}
