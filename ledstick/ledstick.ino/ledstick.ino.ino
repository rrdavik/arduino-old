#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 72
 
#define DATA_PIN 2
#define CLOCK_PIN 3
 
#define LOGLEVEL LOG_LEVEL_DEBUG
CRGB leds[NUM_LEDS];
CRGB colors[7]={CRGB::White,CRGB::Blue,CRGB::Red,CRGB::Yellow,CRGB::Green,CRGB::Orange,CRGB::Purple};

// Práctica encender LPOWER con botón pulsador y luego apagar LPOWER con el mismo
//BPOWER pulsador
const int LPOWER =11;
const int BPOWER = 9;
const int BCHANGESET = 8;
const int Bcolor = 7;



int valColor= 0;
int stateColor = 0;

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

 pinMode(BCHANGESET,INPUT); 
 
 pinMode(Bcolor,INPUT); 
  FastLED.addLeds<LPD8806, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
}


void loop() {       // loop = realice un lazo continuamente
valPower= digitalRead(BPOWER); // lee el estado del 
valColor= digitalRead(Bcolor); // lee el estado del 
if ((valPower == HIGH) && (old_valPower == LOW)){
statePower=1-statePower;
delay(10);
}
old_valPower = valPower; // valPoweror del antiguo estado
if (valColor == HIGH){
stateColor=getColorIndex(stateColor);
delay(10);
}
if (statePower==1){
 digitalWrite(LPOWER, HIGH); // enciende el LPOWER
 readChangeSet();
}
else{
 digitalWrite(LPOWER,LOW); // apagar el LPOWER
 changeSetPower(0);
}
}

int getChangeSetState(int state){
if (state>5){
    state=0;
  } else {
    state++;
  }
  return state;
}

int getColorIndex(int index){
    if (index>7){
    index=0;
  } else {
    index++;
  }
  return index;
  
}


void changeSetPower( int state){

  switch(state){

    case 1:
    program1();
   
     break;
    case 2:
      program2();
     break;
     case 3:
   
      program4();
       break;
         case 4:
      program5();
       break;
   
  case   0:
  default:

     programOff();
   
  }
  
}


void program1(){
  bool b= false;
      for(int i=0; i<NUM_LEDS; i++){
                leds[i] = colors[stateColor]; FastLED.show(); delay(10); 
                leds[i] = CRGB::Black; FastLED.show(); delay(10);
               

           }
}
void program2(){
    bool b= false;
     for(int i=NUM_LEDS; i>0; i--){
                leds[i] = colors[stateColor]; FastLED.show(); delay(10); 
                leds[i] = CRGB::Black; FastLED.show(); delay(10);
//                
//                b= monitorInput();
//                if (b){
//                  break;
//                }
           }
}

void program3(){
  
  int ledcustom[10]={};
  randomSeed(millis());  
  for (int i=0;i<sizeof(ledcustom)-1;i++){
    ledcustom[i] = random(71);
  }
  
     for(int i=0; i<sizeof(ledcustom)-1; i++){
                 leds[ledcustom[i]] =colors[stateColor]; 
                leds[ledcustom[i]] = CRGB::Black; FastLED.show();
                 delay(10);

           }
          
}

void program4(){

      for(int i=0; i<NUM_LEDS/2; i++){
                leds[i] = colors[stateColor]; 
                leds[NUM_LEDS-(i+1)] =colors[stateColor]; FastLED.show();
                delay(10); 
                
                leds[i] = CRGB::Black;
                  leds[NUM_LEDS-(i+1)] = CRGB::Black; FastLED.show();
                delay(10);
               

           }
}

void program5(){

        for(int i=(NUM_LEDS/2); i>0; i--){
                leds[i] =colors[stateColor]; FastLED.show();
                leds[NUM_LEDS-i] = colors[stateColor]; FastLED.show();
                delay(10); 
                
                leds[i] = CRGB::Black;FastLED.show();
                  leds[NUM_LEDS-i] = CRGB::Black; FastLED.show();
                delay(10);
               

           }
}


void programOff(){
   for(int i=0; i<NUM_LEDS; i++){
        leds[i] = CRGB::Black; FastLED.show();
    }
}

void readChangeSet(){
  valChangeset=digitalRead(BCHANGESET);
  if (valChangeset == HIGH){
  stateChangeset=getChangeSetState(stateChangeset);
  delay(10);
  }
  changeSetPower(stateChangeset);

  old_valChangeset = valChangeset;
}


bool monitorInput(){
  bool bbreak = false;
  valChangeset=digitalRead(BCHANGESET);
  valPower= digitalRead(BPOWER);
  if ((valPower == HIGH) && (old_valPower == LOW)){
    statePower=1-statePower;
    delay(10);
    }                 
    if(valChangeset!=old_valChangeset){
      bbreak=true;
     }
return bbreak;
}
