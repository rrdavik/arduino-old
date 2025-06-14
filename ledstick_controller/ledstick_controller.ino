#include <bitswap.h>
#include <chipsets.h>
#include <color.h>
#include <colorpalettes.h>
#include <colorutils.h>
#include <controller.h>
#include <cpp_compat.h>
#include <dmx.h>
#include <FastLED.h>
#include <fastled_config.h>
#include <fastled_delay.h>
#include <fastled_progmem.h>
#include <fastpin.h>
#include <fastspi.h>
#include <fastspi_bitbang.h>
#include <fastspi_dma.h>
#include <fastspi_nop.h>
#include <fastspi_ref.h>
#include <fastspi_types.h>
#include <hsv2rgb.h>
#include <led_sysdefs.h>
#include <lib8tion.h>
#include <noise.h>
#include <pixelset.h>
#include <pixeltypes.h>
#include <platforms.h>
#include <power_mgt.h>

// How many leds in your strip?
#define NUM_LEDS 72
 
#define DATA_PIN 5
#define CLOCK_PIN 3
 
CRGB leds[NUM_LEDS];


// Práctica encender LPOWER con botón pulsador y luego apagar LPOWER con el mismo
//BPOWER pulsador
const int LPOWER =11;
const int BPOWER = 9;
const int BCHANGESET = 8;
const int LDIGIT1 = 2;
const int LDIGIT2 = 4;
const int LDIGIT3 = 7;


int valPower = 0; //valPower se emplea para almacenar el estado del BPOWER
int statePower = 0; // 0 LPOWER apagado, mientras que 1 encendido
int old_valPower = 0; // almacena el antiguo  de valPower
//
int valChangeset = 0; //valChangeset se emplea para almacenar el estado del BCHANGESET
int stateChangeset = 0; // 8 estados... iluminan LDIGIT binario
int old_valChangeset = 0; // almacena el antiguo  de valChangeset


void setup(){  // definir si la variable es de entrada // o salida.
 pinMode(LPOWER,OUTPUT); // establecer que el pin digital es una señal de salida
 pinMode(BPOWER,INPUT); // y BPOWER como señal de entrada
 pinMode(LDIGIT1,OUTPUT);
 pinMode(LDIGIT2,OUTPUT);
 pinMode(LDIGIT3,OUTPUT);
 pinMode(BCHANGESET,INPUT); 
  FastLED.addLeds<LPD8806, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
}


void loop() {       // loop = realice un lazo continuamente
valPower= digitalRead(BPOWER); // lee el estado del 
valChangeset=digitalRead(BCHANGESET);
if ((valPower == HIGH) && (old_valPower == LOW)){
statePower=1-statePower;
delay(10);
}
old_valPower = valPower; // valPoweror del antiguo estado
if (statePower==1){
 digitalWrite(LPOWER, HIGH); // enciende el LPOWER
 if ((valChangeset == HIGH) && (old_valChangeset == LOW)){
  stateChangeset=getChangeSetState(stateChangeset);
  delay(10);
  }
  old_valChangeset = valChangeset; // valPoweror del antiguo estado
  changeSetPower(stateChangeset);
  program1();
}
else{
 digitalWrite(LPOWER,LOW); // apagar el LPOWER
 changeSetPower(0);
}

//valChangeset=digitalRead(BCHANGESET);

}
//
int getChangeSetState(int state){
  if (state>7){
    state=0;
  } else {
    state++;
  }
  return state;
}

void detailPower(int a, int b, int c){
  digitalWrite(LDIGIT1, a); 
  digitalWrite(LDIGIT2, b); 
  digitalWrite(LDIGIT3, c); 
}

void changeSetPower( int state){

  switch(state){

    case 1:
     detailPower(HIGH,LOW,LOW);
     break;
    case 2:
       detailPower(LOW,HIGH,LOW);
     break;
    case 3:
       detailPower(HIGH,HIGH,LOW);
     break;
    case 4:
      detailPower(LOW,LOW,HIGH);
     break;
    case 5:
     detailPower(HIGH,LOW,HIGH);
     break;
   case  6:
     detailPower(LOW,HIGH,HIGH);
     break;
    case 7:
     detailPower(HIGH,HIGH,HIGH);
     break;
  case   0:
  default:

     detailPower(LOW,LOW,LOW);
   
  }
  
}


void program1(){
   for(int i=0; i<NUM_LEDS; i++){
        leds[i] = CHSV(160, 255, 128);
        FastLED.show();
        delay(50);
        leds[i] = CHSV(0,0,0);
        FastLED.show();
    }
}


