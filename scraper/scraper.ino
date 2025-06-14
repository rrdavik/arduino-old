#include <FastLED.h>
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



// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN     4
#define LED_PIN1     5
#define LED_PIN2     6
#define LED_PIN3    7
#define LED_PIN4    8

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 15
#define LED_COUNT1 15
#define LED_COUNT2 60
#define LED_COUNT3 60
#define LED_COUNT4 60

CRGB leds[LED_COUNT];
CRGB leds2[LED_COUNT1];
CRGB strips[] = {
leds,
leds2
//,
// Adafruit_NeoPixel (LED_COUNT2, LED_PIN2, NEO_GRB + NEO_KHZ800),
//Adafruit_NeoPixel  (LED_COUNT3, LED_PIN3, NEO_GRB + NEO_KHZ800),
//Adafruit_NeoPixel  (LED_COUNT4, LED_PIN4, NEO_GRB + NEO_KHZ800)
}
;  








// Buffer used to read LED data from Serial.
// TODO: Direct access to the Adafruit_NeoPixel buffer would cut memory usage in half
char colorValues[2][LED_COUNT*3];

char defaultBright = 50;
  int bufferStrip;
// setup() function -- runs once at startup --------------------------------

void setup() {
    delay(1000);
    Serial.begin(115200);
// Declare our NeoPixel strip object:
 FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, LED_COUNT);
 FastLED.addLeds<NEOPIXEL, LED_PIN1>(leds2, LED_COUNT1);
  FastLED.setDither(false);
  FastLED.setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(255);

 

  // Initialize the buffer to all black
    for (int i=0; i<2; i++) { 
  memset(colorValues[i], 0, sizeof(colorValues[i]));
	}

  while (!Serial) {
    ; // wait for port to be ready
  }
  
  // Tell the computer that we're ready for data
  Serial.println("READY");
}


// loop() function -- runs repeatedly as long as board is on ---------------

void loop() {
 if (Serial.available() > 0) {
    int bufferPos = 0;
    bufferStrip =0;
	while (bufferStrip < 2 ){
		// Read the data for each pixel
		while (bufferPos < LED_COUNT*3) {
		  int color = Serial.read();
		  if (color >= 0) {
			colorValues[bufferStrip][bufferPos++] = color;
		  }
		}
		bufferStrip++;
		bufferPos=0;
    }
	
	bufferStrip =0;
	while (bufferStrip < 2 ){
		// Feed the data to the NeoPixel library
		for(int i=0; i<LED_COUNT; i++) {
		  int d = i*3;
		  
		  strips[bufferStrip][i]= CRGB(255,255,0); //(colorValues[bufferStrip][d], colorValues[bufferStrip][d+1], colorValues[bufferStrip][d+2]);
		}
		bufferStrip++;
	}
  FastLED.show();
    // Clear up the serial buffer
    while (Serial.available() > 0) Serial.read();
    
    // Let the sender know we're ready for more data
    Serial.println("READY");
 } else {
	 Serial.println("shit");
 }
  
}
