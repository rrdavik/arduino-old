  #include "FastLED.h"
  // How many leds in your strip?
#define NUM_LEDS 72
 
#define DATA_PIN 2
#define CLOCK_PIN 3

        CRGB leds[NUM_LEDS];
        void setup() { FastLED.addLeds<LPD8806, DATA_PIN,CLOCK_PIN,RGB>(leds, NUM_LEDS); }
        void loop() { 
           for(int i=0; i<NUM_LEDS; i++){
                leds[i] = CRGB::White; FastLED.show(); delay(30); 
                leds[i] = CRGB::Black; FastLED.show(); delay(30);
           }
        }
