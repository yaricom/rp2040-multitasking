#include <stdio.h>
#include <pico/stdlib.h>
#include <hardware/gpio.h>
#include <hardware/adc.h>

#include <loopTimer.h>
#include <millisDelay.h>

#include <TFT_eSPI.h>

#include "arena.h"
#include "button.h"

#define USER_LED  25
#define TFT_BL    4
#define PWR_ON    22


// LED blinking control
//
bool ledOn = false;
int ledInterval = 500; // ms
millisDelay ledDelay;

int ballsUpdateInterval = 50; // ms
millisDelay ballsUpdateDelay;

#define ARENA_WIDTH   240
#define ARENA_HEIGHT  135
#define MAX_SPEED     5
#define MIN_SPEED     1

#define INIT_BALLS_COUNT 6

Arena arena = Arena(ARENA_WIDTH, ARENA_HEIGHT, MAX_SPEED, MIN_SPEED);

Button leftButton(6);
Button rightButton(7);

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite spr = TFT_eSprite(&tft);  // Declare Sprite object "spr" with pointer to "tft" object

void setup() {
  pinMode(PWR_ON, OUTPUT);
  digitalWrite(PWR_ON, 1);

  pinMode(USER_LED, OUTPUT);

  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, 0);
  analogWrite(TFT_BL, 0);

  // initialize the pushbuttons pins as an input:
  leftButton.begin();
  rightButton.begin();

  Serial.begin(115200);

  adc_init();

  // init TFT
  tft.init();
  tft.setRotation(1);	// landscape
  tft.setSwapBytes(true); // Swap the colour byte order when rendering
  // tft.setTextSize(2);

  // Create a sprite of defined size
  spr.createSprite(ARENA_WIDTH, ARENA_HEIGHT);

  digitalWrite(USER_LED, HIGH);
  analogWrite(TFT_BL, 0xfe); // Turn backlight of the screen

  // init arena
  for (int i = 0; i < INIT_BALLS_COUNT; i++) {
    addRandomBall();
  }
  

  // start timers
  ledDelay.start(ledInterval); 
  ballsUpdateDelay.start(ballsUpdateInterval);

  Serial.println("Setup completed");

  Serial.available();
}

void loop() {
  loopTimer.check(Serial);
  blinkUserLed();
  updateBalls();

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (leftButton.isReleased()) {
    addRandomBall();
  }
  if (rightButton.isReleased()) {
    arena.deleteRandomBall();
  }
}


void blinkUserLed() {
  if (ledDelay.justFinished()) {
    ledDelay.repeat();
    ledOn = !ledOn; 
    digitalWrite(USER_LED, ledOn ? HIGH: LOW);
  }
}

void updateBalls() {
  if (ballsUpdateDelay.justFinished()) {
    ballsUpdateDelay.repeat();

    drawBalls();
  }
}

void drawBalls() {
  // Fill the whole sprite
  spr.fillRect(0, 0, ARENA_WIDTH, ARENA_HEIGHT, TFT_BLACK);

  std::list<Ball> balls = arena.balls();
  int size = balls.size();
  for (Ball const& b : balls) {
    spr.fillEllipse(b.x, b.y, b.radius, b.radius, b.color);
    // tft.drawSmoothCircle(b.x, b.y, b.radius, b.color, TFT_BLACK);
    // Serial.printf(" -> (%d : %d), radius: %d, color: %d\n", b.x, b.y, b.radius, b.radius, b.color);
  }
  arena.update();

  // Now push the sprite to the TFT at position 0,0 on screen
  spr.pushSprite(0, 0);
}

void addRandomBall() {
  uint16_t color = randomBallColor();
  int radius = randoBallRadius();
  arena.addRandomBall(color, radius);
}

uint16_t randomBallColor() {
  byte index = (byte)random(0xff);
  return rainbow(index);
}

int randoBallRadius() {
  return (int)random(5, 10);
}

// -------------------------------------------------------------------------
// Return a 16-bit rainbow colour
// -------------------------------------------------------------------------
uint16_t rainbow(byte value)
{
  // If 'value' is in the range 0-159 it is converted to a spectrum colour
  // from 0 = red through to 127 = blue to 159 = violet
  // Extending the range to 0-191 adds a further violet to red band
 
  value = value%192;
  
  byte red   = 0; // Red is the top 5 bits of a 16-bit colour value
  byte green = 0; // Green is the middle 6 bits, but only top 5 bits used here
  byte blue  = 0; // Blue is the bottom 5 bits

  byte sector = value >> 5;
  byte amplit = value & 0x1F;

  switch (sector)
  {
    case 0:
      red   = 0x1F;
      green = amplit; // Green ramps up
      blue  = 0;
      break;
    case 1:
      red   = 0x1F - amplit; // Red ramps down
      green = 0x1F;
      blue  = 0;
      break;
    case 2:
      red   = 0;
      green = 0x1F;
      blue  = amplit; // Blue ramps up
      break;
    case 3:
      red   = 0;
      green = 0x1F - amplit; // Green ramps down
      blue  = 0x1F;
      break;
    case 4:
      red   = amplit; // Red ramps up
      green = 0;
      blue  = 0x1F;
      break;
    case 5:
      red   = 0x1F;
      green = 0;
      blue  = 0x1F - amplit; // Blue ramps down
      break;
  }
  return red << 11 | green << 6 | blue;
}
