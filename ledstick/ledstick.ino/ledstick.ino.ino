#include <FastLED.h>

#define NUM_LEDS 72
#define DATA_PIN 2
#define CLOCK_PIN 3
#define LOGLEVEL LOG_LEVEL_DEBUG
#define LPOWER 11
#define BCHANGESET 8
#define BPUSH 10
#define BCOLOR 7
#define SWB 4
#define SWR 5
#define NLEDSRANDOM 5
#define UPDATES_PER_SECOND 2

struct Program {
  int id;
  CRGB color;
  int program;
  int index;
};

CRGB leds[NUM_LEDS];
int colors[7] = { 100, 0, 95,  160,  45, 11, 210};
int vCol, lCol, sCol = 0; // color
int vProg, lProg = 0; // prog
int  sProg = 1;
int vPush, lPush = 0;
Program programs[20];
int removeProgs[20];
int ledcustom[NLEDSRANDOM];
int progIndex, remIndex = 0;
int sBright, sRate;

void setup() {
  delay(1000);
  pinMode(LPOWER, OUTPUT);
  pinMode(SWB, OUTPUT);
  pinMode(SWR, OUTPUT);
  pinMode(BCHANGESET, INPUT);
  pinMode(BPUSH, INPUT);
  pinMode(BCOLOR, INPUT);
  FastLED.addLeds<LPD8806, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setDither(false);
  FastLED.setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(255);
}
void loop() {
  digitalWrite(SWB, HIGH);
  vCol = digitalRead(BCOLOR);
  vPush = digitalRead(BPUSH);
  vProg = digitalRead(BCHANGESET);
  sBright = readBright();
  sRate = readRate();
  digitalWrite(SWR, LOW);
  getColor();
  getProgram();
  pushProgram();
  pushLight();
}
int readBright() {
  return analogRead(0);
}
int readRate() {
  digitalWrite(SWB, LOW);
  delay(20);
  digitalWrite(SWR, HIGH);
  return  (analogRead(0) / 10) + 1;
}
void pushProgram() {
  if (vPush == HIGH && progIndex < 19) {
    addProgram();
  }
  lPush = vPush;
}
void addProgram() {
  Program p;
  p.color = CHSV(  colors[sCol], (sCol == 0) ? 0 : 255, getBright());
  p.program = sProg;
  p.index = 0;
  p.id = progIndex;
  programs[progIndex++] = p;
}
void pushLight() {
  digitalWrite(LPOWER, HIGH); // power on
  FastLED.delay(1000 / (UPDATES_PER_SECOND * sRate  ));

  for (int i = 0; i < progIndex; i++)
    runProgram(&programs[i], (programs[i].program != 5) ? 1 : 0);

  FastLED.show();
  for (int i = 0; i < progIndex; i++)
    if (programs[i].program != 3 || programs[i].program != 5) {
      runProgram(&programs[i], 0);
    }
  FastLED.show();

  for (int i = 0; i < remIndex; i++)
    removeProgram(removeProgs[i]);
  remIndex = 0;
}
void removeProgram(int id) {
  for (int i = id; i < progIndex; i++) {
    programs[i] = programs[i + 1];
    programs[i].id--;
  }
  progIndex--;
  if (progIndex == 0) {
    digitalWrite(LPOWER, LOW); // power on
  }
}
void  getColor() {
  if (vCol == HIGH && lCol == LOW)
    getColorIndex();
  lCol = vCol;
}
void getProgram() {
  if (vProg == HIGH  && lProg == LOW)
    readProgram();
  lProg = vProg;
}
void readProgram() {
  if (sProg >= 6)
    sProg = 1;
  else
    sProg++;
}
void getColorIndex() {
  if (sCol > 5)
    sCol = 0;
  else
    sCol++;
}
int getBright() {
  Serial.print("sRate ");
  Serial.print(sRate);
  Serial.print(" Bright ");
  Serial.println(sBright );
  return (int)(sBright * 255 / 1024);
}
void lightsOn(Program* p) {
  switch (p->program) {
    case 1:
      program1(p);
      break;
    case 2:
      program2(p);
      break;
    case 3:
      program3(p);
      break;
    case 4:
      program4(p);
      break;
    case 6:
      program6(p);
    default: ;
  }
}
void lightsOff(Program* p) {
  switch (p->program) {
    case 1:
      program1Off(p);
      break;
    case 2:
      program2Off(p);
      break;
    case 4:
      program4Off(p);
    default: ;
  }
}
void pushLightStep(Program* p, int o) {
  if (o == 1)
    lightsOn(p);
  else {
    lightsOff(p);
    p->index++;
  }
}
void runProgram(Program* p, int o) {
  switch (p->program) {
    case 1:
    case 2:
    case 3:
    case 6:
      if ( p->index < NUM_LEDS)
        pushLightStep(p, o);
      else  if (  o == 0)
        removeProgs[remIndex++] = p->id;
      break;
    case 4:
      if ( p->index < NUM_LEDS / 2)
        pushLightStep(p, o);
      else   if (  o == 0)
        removeProgs[remIndex++] = p->id;
      break;
    case 5:
      program5(p);
    default: ;
  }
}
void program1(Program* p) {
  leds[p->index] = p->color;
}
void program1Off(Program* p) {
  leds[p->index] = CHSV( 0, 0, 0);
}
void program2(Program* p) {
  leds[NUM_LEDS - (p->index + 1)] = p->color;
}
void program2Off(Program* p) {
  leds[NUM_LEDS - (p->index + 1)] = CHSV( 0, 0, 0);
}
void program3(Program* p) {
  if ( p->index % 2 == 0) {
    randomSeed(millis());
    for (int i = 0; i < NLEDSRANDOM; i++) {
      ledcustom[i] = random(73);
      leds[ledcustom[i]] = p->color;
    }
  } else {
    for (int i = 0; i < NLEDSRANDOM; i++)
      leds[ledcustom[i]] =  CHSV( 0, 0, 0);
  }
}
void program4(Program* p) {
  leds[p->index] = leds[NUM_LEDS - (p->index + 1)] = p->color;
}
void program4Off(Program* p) {
  leds[p->index] =  leds[NUM_LEDS - (p->index + 1)] = CHSV( 0, 0, 0);
}
void program5(Program* p) {
  if ( p->index < NUM_LEDS) {
    leds[p->index] = p->color;
    p->index++;
  } else if (p->index < 2 * NUM_LEDS) {
    leds[p->index - NUM_LEDS] =   CHSV( 0, 0, 0);
    p->index++;
  } else
    removeProgs[remIndex++] = p->id;
}
void program6(Program* p) {
  if (p->index == 0) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV( 0, 0, 0);
      FastLED.show();
    }
  }
  leds[p->index] = p->color;
}
