// A basic everyday NeoPixel strip test program.

// NEOPIXEL BEST PRACTICES for most reliable operation:
// - Add 1000 uF CAPACITOR between NeoPixel strip's + and - connections.
// - MINIMIZE WIRING LENGTH between microcontroller board and first pixel.
// - NeoPixel strip's DATA-IN should pass through a 300-500 OHM RESISTOR.
// - AVOID connecting NeoPixels on a LIVE CIRCUIT. If you must, ALWAYS
//   connect GROUND (-) first, then +, then data.
// - When using a 3.3V microcontroller with a 5V-powered NeoPixel strip,
//   a LOGIC-LEVEL CONVERTER on the data line is STRONGLY RECOMMENDED.
// (Skipping these may work OK on your workbench but can fail in the field)

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN     12
#define LED_PIN1     11
#define LED_PIN2     10
#define LED_PIN3    9
#define LED_PIN4    8

// How many NeoPixels are attached to the Arduino?
#define N_STRIPS 5
#define LED_COUNT 60
//#define LED_COUNT1 30
//#define LED_COUNT2 6
//#define LED_COUNT3 6
//#define LED_COUNT4 6


// Declare our NeoPixel strip object:

Adafruit_NeoPixel strips[] = {
  Adafruit_NeoPixel (LED_COUNT, LED_PIN, NEO_GRBW  + NEO_KHZ800 ),
  Adafruit_NeoPixel  (LED_COUNT, LED_PIN1, NEO_GRBW + NEO_KHZ800),
    Adafruit_NeoPixel  (LED_COUNT, LED_PIN2, NEO_GRBW + NEO_KHZ800),
      Adafruit_NeoPixel  (LED_COUNT, LED_PIN3, NEO_GRBW + NEO_KHZ800),
        Adafruit_NeoPixel  (LED_COUNT, LED_PIN4, NEO_GRBW + NEO_KHZ800)
  // Adafruit_NeoPixel (LED_COUNT, LED_PIN2, NEO_GRB + NEO_KHZ800),
  //Adafruit_NeoPixel  (LED_COUNT, LED_PIN3, NEO_GRB + NEO_KHZ800),
  //Adafruit_NeoPixel  (LED_COUNT, LED_PIN4, NEO_GRB + NEO_KHZ800)
};
int bufferStrip;


// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

// Buffer used to read LED data from Serial.
// TODO: Direct access to the Adafruit_NeoPixel buffer would cut memory usage in half
char colorValues[N_STRIPS][LED_COUNT * 3];

char defaultBright = 50;



void setup() {

  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.
  
  Serial.begin(115200);

  bufferStrip = 0;

  while (bufferStrip < N_STRIPS ) {
    // update the strip
    strips[bufferStrip].begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
    strips[bufferStrip].show();
    strips[bufferStrip++].setBrightness(50);
  }

  // Do a quick test/demo to show that things are working
  //colorWipe(strips[0].Color(255,   0,   0), 10); // Red

  // Initialize the buffer to all black
  for (int i = 0; i < N_STRIPS; i++) {
    memset(colorValues[i], 0, sizeof(colorValues[i]));
  }

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

}


void loop() {

  // if we get a valid byte, read analog ins:
  if (Serial.available() > 0) {

    int bufferPos = 0;
    int bufferStrip = 0;
    while (bufferStrip < N_STRIPS) {
      // Read the data for each pixel
      while (bufferPos < LED_COUNT * 3 ) {
        // get incoming byte:
        int inByte =  Serial.read();
        if (inByte >= 0) {
          colorValues[bufferStrip][bufferPos++] = inByte;
        }
      }
      for (int i = 0; i < LED_COUNT; i++) {
        int d = i * 3;
     //  uint32_t c = strips[bufferStrip].Color(colorValues[bufferStrip][d], colorValues[bufferStrip][d + 1], colorValues[bufferStrip][d + 2]);
        strips[bufferStrip].setPixelColor(i, colorValues[bufferStrip][d], colorValues[bufferStrip][d + 1], colorValues[bufferStrip][d + 2],50);
      }

       strips[bufferStrip++].show();
      bufferPos = 0;
    }
  }
}



// Some functions of our own for creating animated effects -----------------

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  bufferStrip=0;
  while (bufferStrip < N_STRIPS ) {
    for (int i = 0; i < LED_COUNT; i++) { // For each pixel in strip...
      strips[bufferStrip].setPixelColor(i, color);         //  Set pixel's color (in RAM)

      strips[bufferStrip].show();                          //  Update strip to match

      delay(wait);                           //  Pause for a moment
    }
    bufferStrip++;
  }
}
