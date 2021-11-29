// Simple strand test for Adafruit Dot Star RGB LED strip.
// This is a basic diagnostic tool, NOT a graphics demo...helps confirm
// correct wiring and tests each pixel's ability to display red, green
// and blue and to forward data down the line.  By limiting the number
// and color of LEDs, it's reasonably safe to power a couple meters off
// the Arduino's 5V pin.  DON'T try that with other code!

//#include <Adafruit_DotStar.h>
// Because conditional #includes don't work w/Arduino sketches...
#include <SPI.h>         // COMMENT OUT THIS LINE FOR GEMMA OR TRINKET
//#include <avr/power.h> // ENABLE THIS LINE FOR GEMMA OR TRINKET

// defines

#include <FastLED.h>
#include "colorutils.h"
#include "colorpalettes.h"
#include <pixeltypes.h>

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

//This is where we adjust things to match our unique project:
#define NUM_LEDS    288      // adjust this to the number of LEDs you have: 16 or more
#define LED_TYPE    DOTSTAR  //APA102 // adjust this to the type of LEDS. This is for Neopixels
#define DATA_PIN    11       // adjust this to the pin you've connected your LEDs to 
#define CLOCK_PIN    13  
#define MAX_BRIGHTNESS  150      // 255 is full brightness, 128 is half
#define MIN_BRIGHTNESS  10
#define SATURATION  255     // 0-255, 0 is pure white, 255 is fully saturated color
#define BUTTON_PIN  2      // Connect the button to GND and one of the pins.
#define UPDATES_PER_SECOND 100
#define COLOR_ORDER BGR
#define NUM_MODES 9 //Update this number to the highest number of "cases"

uint8_t gHue = 0; // rotating "base color" used by many of the patterns

uint16_t STEPS = 30;// STEPS set dynamically once we've started up
uint16_t SPEED = 30;// SPEED set dynamically once we've started up

CRGB leds[NUM_LEDS];
CRGBPalette16 currentPalette;
TBlendType    currentBlending;

int ledMode = 0;

uint8_t colorLoop = 1;

const TProgmemPalette16 MyColors_p PROGMEM =
{
  CRGB:: DarkBlue,
  CRGB:: HotPink,
  CRGB:: Teal,
  CRGB:: BlueViolet,
 
  CRGB:: DodgerBlue,    
  CRGB:: DeepPink,
  CRGB:: Turquoise,
  CRGB:: Indigo,
 
  CRGB:: DarkBlue,
  CRGB:: HotPink,
  CRGB:: Teal,
  CRGB:: BlueViolet,
 
  CRGB:: DodgerBlue,    
  CRGB:: DeepPink,
  CRGB:: Turquoise,
  CRGB:: Indigo,
};

byte prevKeyState = HIGH;         // button is active low

const int brightnessInPin = A9;  // The Analog input pin that the brightness control potentiometer is attached to.
const int yButtonPin = 2;
//const int ledPin = 11;  // the number of the LED pin
int sensorValue = 0;
int mappedValue = 0;
//int ledState = HIGH;         // the current state of the output pin
int yButtonState;             // the current reading from the input pin
int yLastButtonState = LOW;   // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers


//------------------SETUP------------------
void setup() {
  delay( 2000 ); // power-up safety delay
  FastLED.addLeds<DOTSTAR, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS);  // BGR ordering is typical
  // FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);  // BGR ordering is typical
  //FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.setBrightness( BRIGHTNESS );
 
  currentBlending = LINEARBLEND; //all of these will be blended unless I tell them not to be

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  FastLED.setBrightness(32); // start off 1/8 brightness
  FastLED.clear();
}

//------------------MAIN LOOP------------------
void loop() {
SPI.begin();
  //dimmer knob
int sensorValue = analogRead(brightnessInPin);
// map to brightness range
int mappedValue = map(sensorValue, 0, 1023, 0, 255);
//strip.setBrightness(mappedValue);
FastLED.setBrightness(constrain(mappedValue, MIN_BRIGHTNESS, MAX_BRIGHTNESS));
//Serial.print("A3: ");
//Serial.println(sensorValue);

  byte currKeyState = digitalRead(BUTTON_PIN);

  if ((prevKeyState == LOW) && (currKeyState == HIGH)) {
    shortKeyPress();
  }
  prevKeyState = currKeyState;

  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */

  switch (ledMode) {

//FastLED has a bunch of built-in "palettes" to choose from:
//RainbowColors_p     is all the colors of the rainbow
//PartyColors_p     is all the colors of the rainbow minus greens
//RainbowStripeColors_p is all the colors of the rainbow divided into stripes
//HeatColors_p      is reds and yellows, white, and black
//LavaColors_p      is more reds and orangey colors
//ForestColors_p    is greens and yellows
//OceanColors_p     is lots of blues and aqua colors
//CloudColors_p     is blues and white
//MyColors_p        is whatever you define above

//The group of colors in a palette are sent through a strip of LEDS in speed and step increments youve chosen
//You can change the SPEED and STEPS to make things look exactly how you want
//SPEED refers to how fast the colors move.  Higher numbers = faster motion
//STEPS refers to how wide the bands of color are. Based on the palette & number of LEDs, some look better at different steps

  case 0:
  {fill_solid( currentPalette, 16, CRGB::Gold); currentBlending = NOBLEND;}
  //{currentPalette = RainbowColors_p;  SPEED = 100;  STEPS = 16;}
  break;
  
  case 1: 
  {fill_solid( currentPalette, 16, CRGB::DarkOrange); currentBlending = NOBLEND;}
  //{currentPalette = RainbowStripeColors_p;    SPEED = 50;   STEPS = 16; currentBlending = NOBLEND;} 
  break;

  case 2:
  {fill_solid( currentPalette, 16, CRGB::Lime); currentBlending = NOBLEND;}
  //{currentPalette = PartyColors_p;   SPEED = 150;   STEPS = 48; currentBlending = LINEARBLEND;}
  break;

  case 3:
  {fill_solid( currentPalette, 16, CRGB::Aqua); currentBlending = NOBLEND;}
  //{currentPalette = MyColors_p;     SPEED = 75;   STEPS = 48;} 
  break;

  case 4:
  {fill_solid( currentPalette, 16, CRGB::Peru); currentBlending = NOBLEND;}
  //{currentPalette = HeatColors_p;     SPEED = 150;   STEPS = 16;} 
  break;

  case 5:
  {fill_solid( currentPalette, 16, CRGB::GreenYellow); currentBlending = NOBLEND;}
  //{SetupWatermelonPalette(); SPEED = 125; STEPS = 8;}
  break;

  case 6:
  {fill_solid( currentPalette, 16, CRGB::Seashell); currentBlending = NOBLEND;}
  //{currentPalette = ForestColors_p;     SPEED = 200;   STEPS = 48;} //all steps look good. Speed should be high.
  break;

  case 7:
  {fill_solid( currentPalette, 16, CRGB::Purple); currentBlending = NOBLEND;}
  //{currentPalette = OceanColors_p;    SPEED = 100;   STEPS = 16;} //
  break; 
 
  case 8:
  {fill_solid( currentPalette, 16, CRGB::Crimson); currentBlending = NOBLEND;}
  //{SetupNewPalette(); SPEED = 250; STEPS = 16; currentBlending = NOBLEND;} 
  break;

   case 9:
  {fill_solid( currentPalette, 16, CRGB::Red); currentBlending = NOBLEND;}
  break;
  } 

  FillLEDsFromPaletteColors( startIndex);
  FastLED.show();
  //FastLED.delay(1000 / SPEED);  
  FastLED.delay(1);
}

void shortKeyPress() {
  ledMode++;
  if (ledMode > NUM_MODES) {
    ledMode=0; 
  }  
}

void FillLEDsFromPaletteColors( uint8_t colorIndex) {
  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, MAX_BRIGHTNESS, currentBlending);
    colorIndex += STEPS;              
  }
}

void SetupNewPalette()
{
    fill_solid( currentPalette, 16, CRGB::Black);
    //   set half of the LEDs to the color selected here
    //  Play with the color, steps, and speed to get different results. 
    currentPalette[0] = CRGB::DodgerBlue;
    currentPalette[1] = CRGB::DodgerBlue;
    currentPalette[2] = CRGB::DodgerBlue;
    currentPalette[3] = CRGB::DodgerBlue;
    
//    currentPalette[4] = CRGB::DodgerBlue;
//    currentPalette[5] = CRGB::DodgerBlue;
//    currentPalette[6] = CRGB::DodgerBlue;
//    currentPalette[7] = CRGB::DodgerBlue;
    
    currentPalette[8] = CRGB::DodgerBlue;
    currentPalette[9] = CRGB::DodgerBlue;
    currentPalette[10] = CRGB::DodgerBlue;
    currentPalette[11] = CRGB::DodgerBlue;

//    currentPalette[12] = CRGB::DodgerBlue;
//    currentPalette[13] = CRGB::DodgerBlue;
//    currentPalette[14] = CRGB::DodgerBlue;
//    currentPalette[15] = CRGB::DodgerBlue;
 
}

//  This function sets up a palette of purple and green stripes.
//  Play with the color, steps, and speed to get different results. 
void SetupWatermelonPalette()
{
    CRGB Pink = CHSV(HUE_PINK, 255, 255);
    CRGB Green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   Green,  Green,  Green,  Green,
                                   Pink, Pink,  Pink, Pink,
                                   Green,  Green,  Green,  Green,
                                   Pink, Pink,  Pink, Pink );
}
